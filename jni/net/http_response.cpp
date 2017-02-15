#include "http_response.h"



godin::HttpResponse::HttpResponse(std::unique_ptr<godin::HttpRequest> request) {

  http_request_ = std::move(request);
  succeed_ = false;

  response_header_.clear();
  response_data_.clear();
  error_buffer_.clear();
}
