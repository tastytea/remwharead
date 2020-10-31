#ifndef REMWHAREAD_CURL_WRAPPER_HPP
#define REMWHAREAD_CURL_WRAPPER_HPP

#include "curl/curl.h"

#include <string>
#include <string_view>

namespace remwharead
{

using std::string;
using std::string_view;

class CURLWrapper
{
public:
    /*!
     *  @brief  Initializes curl and sets up connection.
     *
     *  The first time an instance of CURLWrapper is created, it calls
     *  `curl_global_init`, which is not thread-safe. For more information
     *  consult [curl_global_init(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_init.html).
     *
     *  @since  0.11.0
     */
    CURLWrapper();

    /*!
     *  @brief  Copy constructor. Does the same as the Constructor.
     *
     *  @since  0.11.0
     */
    CURLWrapper(const CURLWrapper &);

    //! Move constructor
    CURLWrapper(CURLWrapper &&other) noexcept = delete;

    /*!
     *  @brief  Cleans up curl and connection.
     *
     *  May call `curl_global_cleanup`, which is not thread-safe. For more
     *  information consult [curl_global_cleanup(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_cleanup.html).
     *
     *  @since  0.11.0
     */
    virtual ~CURLWrapper() noexcept;

    //! Copy assignment operator
    CURLWrapper &operator=(const CURLWrapper &other) = delete;

    //! Move assignment operator
    CURLWrapper &operator=(CURLWrapper &&other) noexcept = delete;

    /*!
     *  @brief  Returns pointer to the CURL easy handle.
     *
     *  You can use this handle to set or modify curl options. For more
     *  information consult [curl_easy_setopt(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_setopt.html).
     *
     *  @since  0.11.0
     */
    inline CURL *get_curl_easy_handle()
    {
        return _connection;
    }

    /*!
     *  @brief  URL encodes the given string.
     *
     *  For more information consult [curl_easy_escape(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_escape.html).
     *
     *  @param  url String to escape.
     *
     *  @return The escaped string or {} if it failed.
     *
     *  @since  0.11.0
     */
    [[nodiscard]] inline string escape_url(const string_view url) const
    {
        char *cbuf{curl_easy_escape(_connection, url.data(),
                                    static_cast<int>(url.size()))};
        string sbuf{cbuf};
        curl_free(cbuf);
        return sbuf;
    }

    /*!
     *  @brief  URL decodes the given string.
     *
     *  For more information consult [curl_easy_unescape(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_unescape.html).
     *
     *  @param  url String to unescape.
     *
     *  @return The unescaped string or {} if it failed.
     *
     *  @since  0.11.0
     */
    [[nodiscard]] inline string unescape_url(const string_view url) const
    {
        char *cbuf{curl_easy_unescape(_connection, url.data(),
                                      static_cast<int>(url.size()), nullptr)};
        string sbuf{cbuf};
        curl_free(cbuf);
        return sbuf;
    }

    /*!
     *  @brief  Make a HTTP request.
     *
     *  @param  uri     The full URI.
     *  @param  archive Archive URI instead of fetching the body.
     *
     *  @return The body of the page or the URI of the archived page.
     *
     *  @since  0.11.0
     */
    [[nodiscard]] string make_request(string uri, bool archive);

    /*!
     *  @brief  Returns a reference to the buffer libcurl writes into.
     *
     *  @since  0.11.0
     */
    [[nodiscard]] inline string &get_buffer()
    {
        return _buffer_body;
    }

private:
    CURL *_connection;
    char _buffer_error[CURL_ERROR_SIZE]{};
    string _buffer_headers;
    string _buffer_body;

    /*!
     *  @brief  libcurl write callback function.
     *
     *  @since  0.11.0
     */
    size_t writer_body(char *data, size_t size, size_t nmemb);

    /*!
     *  @brief  Wrapper for curl, because it can only call static member
     *          functions.
     *
     *  <https://curl.haxx.se/docs/faq.html#Using_C_non_static_functions_f>
     *
     *  @since  0.11.0
     */
    static inline size_t writer_body_wrapper(char *data, size_t sz,
                                             size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper *>(f)->writer_body(data, sz, nmemb);
    }

    //! @copydoc writer_body
    size_t writer_headers(char *data, size_t size, size_t nmemb);

    //! @copydoc writer_body_wrapper
    static inline size_t writer_headers_wrapper(char *data, size_t sz,
                                                size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper *>(f)->writer_headers(data, sz, nmemb);
    }
};

} // namespace remwharead

#endif // REMWHAREAD_CURL_WRAPPER_HPP
