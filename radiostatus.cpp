#include "radiostatus.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <mpg123.h>
#include <portaudio.h>

RadioStatus::RadioStatus(QObject *parent)
    : QObject(parent),
    playing(false),
    paused(false),
    terminate(false),
    stream(nullptr),
    mh(nullptr)
{
    std::cout << "RadioStatus created" << std::endl;

    PaError err = Pa_Initialize();
    if (err != paNoError)
        std::cerr << "PortAudio init error: " << Pa_GetErrorText(err) << std::endl;

    int mpgErr;
    mpg123_init();
    mh = mpg123_new(nullptr, &mpgErr);
    if (!mh) {
        std::cerr << "mpg123 init error: " << mpg123_plain_strerror(mpgErr) << std::endl;
    }
    mpg123_open_feed(mh);
}

RadioStatus::~RadioStatus()
{
    stop();

    if (mh) {
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
    }

    Pa_Terminate();
    std::cout << "RadioStatus destroyed" << std::endl;
}

void RadioStatus::play(const QString &url)
{
    if (playing) {
        std::cerr << "Already playing!" << std::endl;
        return;
    }

    this->url = url;
    terminate = false;
    playing = true;
    paused = false;

    streamThread = std::thread(&RadioStatus::streamThreadFunc, this, url.toStdString());

    emit playingChanged();
    emit currentUrlChanged();
}

void RadioStatus::pause()
{
    if (!playing) return;
    paused = true;
    emit pausedChanged();
}

void RadioStatus::resume()
{
    if (!playing || !paused) return;
    paused = false;
    emit pausedChanged();
}

void RadioStatus::stop()
{
    if (!playing) return;
    terminate = true;
    if (streamThread.joinable())
        streamThread.join();

    playing = false;
    paused = false;
    emit playingChanged();
    emit pausedChanged();
}

bool RadioStatus::isPlaying() const { return playing; }
bool RadioStatus::isPaused() const { return paused; }
QString RadioStatus::currentUrl() const { return url; }

bool RadioStatus::openStream()
{
    if (stream) return true;

    PaError err;
    err = Pa_OpenDefaultStream(&stream,
                               0,
                               2,
                               paInt16,
                               44100,
                               1024,
                               nullptr,
                               nullptr);
    if (err != paNoError) {
        std::cerr << "PortAudio open stream error: " << Pa_GetErrorText(err) << std::endl;
        stream = nullptr;
        return false;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio start stream error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    return true;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    auto *buffer = static_cast<std::vector<unsigned char>*>(userp);
    size_t totalSize = size * nmemb;
    buffer->insert(buffer->end(), (unsigned char*)contents, (unsigned char*)contents + totalSize);
    return totalSize;
}

void RadioStatus::streamThreadFunc(const std::string &url)
{
    std::cout << "Starting stream from: " << url << std::endl;

    if (!openStream()) {
        std::cerr << "Failed to open audio stream!" << std::endl;
        return;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return;
    }

    // This callback will be called repeatedly as data arrives
    auto writeCallback = [](void *contents, size_t size, size_t nmemb, void *userp) -> size_t {
        auto *self = static_cast<RadioStatus*>(userp);
        size_t totalSize = size * nmemb;

        if (self->terminate) return 0; // Stop the transfer

        // Wait while paused
        while (self->paused && !self->terminate) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (self->terminate) return 0;

        // Feed data to mpg123
        int ret = mpg123_feed(self->mh, (unsigned char*)contents, totalSize);
        if (ret != MPG123_OK) {
            std::cerr << "mpg123_feed error: " << mpg123_plain_strerror(ret) << std::endl;
        }

        // Decode and play
        unsigned char *audio = nullptr;
        size_t bytesDecoded = 0;
        long rate = 0;
        int channels = 0, encoding = 0;

        int decodeRet;
        while ((decodeRet = mpg123_decode_frame(self->mh, nullptr, &audio, &bytesDecoded)) == MPG123_OK) {
            if (mpg123_getformat(self->mh, &rate, &channels, &encoding) == MPG123_OK) {
                if (self->stream && audio && bytesDecoded > 0) {
                    // Calculate frames: bytesDecoded / (channels * bytes_per_sample)
                    // For paInt16: 2 bytes per sample
                    unsigned long frames = bytesDecoded / (channels * 2);
                    PaError err = Pa_WriteStream(self->stream, audio, frames);
                    if (err != paNoError) {
                        std::cerr << "Pa_WriteStream error: " << Pa_GetErrorText(err) << std::endl;
                    }
                }
            }
        }

        // MPG123_NEED_MORE is normal, means we need more data
        if (decodeRet != MPG123_NEED_MORE && decodeRet != MPG123_OK) {
            std::cerr << "mpg123_decode_frame error: " << mpg123_plain_strerror(decodeRet) << std::endl;
        }

        return totalSize;
    };

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L); // No timeout for streaming
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); // Thread-safe

    std::cout << "Starting CURL transfer..." << std::endl;
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK && res != CURLE_ABORTED_BY_CALLBACK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
    }

    std::cout << "Stream ended" << std::endl;

    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        stream = nullptr;
    }

    curl_easy_cleanup(curl);
}
