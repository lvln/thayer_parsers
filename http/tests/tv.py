import random

# Valid methods to test
valid_methods = ["GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"]

# Invalid http methods
invalid_methods = ["GES", "HEAS", "PSST", "PUTT", "DELET", "CONNEC", "OPTION", "TRACES"]

# Valid URLs
valid_urls = ["/a/b/", "http://a/b/c/..g", "http://example.com", "http://example.com/data", "http://example.com/data/", "example.com:80/~smith/home.html", "http://g", "http://a/b/c/.g", "http://www.example.com/hello.txt", "http://www.example.org:8080/pub/WWW/TheProject.html", "http://www.ietf.org/rfc/rfc2396.txt", "/g", "http://a/b/c/d;p?q", "http://a/b/c/g.", "/", "*"]

# Invalid URLs
invalid_urls = ["?y", "mailto:John.Doe@example.com", "tel:+1-816-555-1212", "ftp://ftp.is.co.za/rfc/rfc1808.txt#", "g", "..", "g?y#s", "g?y/./x", "ldap://[2001:db8::7]/c=GB?objectClass?one", "**"]

# Valid versions
valid_versions = ["HTTP/1.1", "HTTP/2.0", "HTTP/1.0"]

# Invalid versions
invalid_versions = ["HTTP1.1", "HTTP/2.", "HTTP/.0", "HTT/1.0"]

# Valid headers
valid_headers = ["Accept: text/html,\r\n application/xhtml+xml,\r\n application/xml;q=0.9,\r\n */*;q=0.8\r\n", "Accept-Charset:utf-8,\r\n iso-8859-1;q=0.5\r\n", "Accept-Encoding: gzip,\r\n  deflate,\r\n   br\r\n", "Accept-Language: en-US\r\n", "Authorization: Basic XMlcrpwYzdyZA==\r\n", "Connection: keep-alive\r\n", "Content-Length: 123\r\n", "Content-Type: application/json\r\n", "Date: Tue, 23 Jul 2024 12:00:00 GMT\r\n", "Host: www.example.com\r\n", "Pragma:\r\n", "Range: \r\n", "Referer: https://www.example.com/page\r\n", "Transfer-Encoding: chunked\r\n", "Vary:     \r\n", "WWW-Authenticate: Basic realm=\"Access to the staging site\"\r\n"]

# Invalid headers
invalid_headers = ["Accept: text/html,\r\n application/xhtml+xml,\r\n application/xml;q=0.9,\r\n*/*;q=0.8\r\n", "Accept-Charset :utf-8,\r\n iso-8859-1;q=0.5\r\n", "Accept-Encoding :gzip, deflate, br\r\n", "Accept-Language: en-US\n", "Authorization:Basic XMlcrpwYzdyZA==\r", "Connection : keep-alive\r\n", "Content-Length : 123\r\n", "Content-Type: application\r\n/json\r\n", "Date Tue, 23 Jul 2024 12:00:00 GMT\r\n", "Pragma :\r\n", " Range: \r\n", "Referer https://www.example.com/page\r\n", ": chunked\r\n", ":\r\n", "WWW-Authenticate    : Basic realm=\"Access to the staging site\"\r\n"]

# Valid status codes
valid_status_codes = ["100 Continue", "101 Switching Protocols", "102 Processing", "103 Early Hints", "200 OK", "201 Created", "202 Accepted", "203 Non-Authoritative Information", "204 No Content", "205 Reset Content", "206 Partial Content", "207 Multi-Status", "208 Already Reported", "226 IM Used", "300 Multiple Choices", "301 Moved Permanently", "302 Found", "303 See Other", "304 Not Modified", "307 Temporary Redirect", "308 Permanent Redirect", "400 Bad Request", "401 Unauthorized", "402 Payment Required", "403 Forbidden", "404 Not Found", "405 Method Not Allowed", "406 Not Acceptable", "407 Proxy Authentication Required", "408 Request Timeout", "409 Conflict", "410 Gone", "411 Length Required", "412 Precondition Failed", "413 Payload Too Large", "414 URI Too Long", "415 Unsupported Media Type", "416 Range Not Satisfiable", "417 Expectation Failed", "418 I'm a teapot", "421 Misdirected Request", "422 Unprocessable Content", "423 Locked", "424 Failed Dependency", "425 Too Early", "426 Upgrade Required", "428 Precondition Required", "429 Too Many Requests", "431 Request Header Fields Too Large", "451 Unavailable For Legal Reasons", "500 Internal Server Error", "501 Not Implemented", "502 Bad Gateway", "503 Service Unavailable", "504 Gateway Timeout", "505 HTTP Version Not Supported", "506 Variant Also Negotiates", "507 Insufficient Storage", "508 Loop Detected", "510 Not Extended", "511 Network Authentication Required"]

# Invalid status codes
invalid_status_codes = ["600 Continue", "701 Switching Protocols", "802 Processing", "903 Early Hints", "2000 OK", "001 Created", "Accepted", "203", "204 "]

def write_pass_request(method, url, version, headers, pass_number, message_body):
    if headers != None:
        header_string = "".join(headers)
    else:
        header_string = ""
        
    test_string = f"{method} {url} {version}\r\n{header_string}\r\n{message_body}"
    with open(f"pass.{pass_number}", "w") as f:
        f.write(test_string)

def write_fail_request(method, url, version, headers, fail_number, message_body):
    if headers != None:
        header_string = "".join(headers)
    else:
        header_string = ""
        
    test_string = f"{method} {url} {version}\r\n{header_string}\r\n{message_body}"
    with open(f"fail.{fail_number}", "w") as f:
        f.write(test_string)

def write_pass_response(version, status, headers, pass_number, message_body):
    if headers != None:
        header_string = "".join(headers)
    else:
        header_string = ""
        
    test_string = f"{version} {status}\r\n{header_string}\r\n{message_body}"
    with open(f"pass.{pass_number}", "w") as f:
        f.write(test_string)

def write_fail_response(version, status, headers, fail_number, message_body):
    if headers != None:
        header_string = "".join(headers)
    else:
        header_string = ""
        
    test_string = f"{version} {status}\r\n{header_string}\r\n{message_body}"
    with open(f"fail.{fail_number}", "w") as f:
        f.write(test_string)

def request_body(method):
    if method in ["POST", "PUT"]:
        return "key1=value1&key2=value2" 
    elif method == "PATCH":
        return '{"field": "value"}'
    elif method == "DELETE":
        return "Record to delete"
    else:
        return ""

def response_body(status_code):
    if status_code == 200:
        return "Your request was successful."
    elif status_code == 201:
        return "Resource has been created."
    elif status_code == 400:
        return "The request could not be understood or was missing required parameters."
    elif status_code == 401:
        return "Authentication credentials were missing or incorrect."
    elif status_code == 403:
        return "You do not have permission to access the requested resource."
    elif status_code == 404:
        return "The requested resource could not be found."
    elif status_code == 500:
        return "An unexpected error occurred on the server."
    elif status_code == 502:
        return "The server received an invalid response from the upstream server."
    elif status_code == 503:
        return "The server is currently unable to handle the request due to maintenance or overload."
    else:
        return ""

# Generate all combinations of method, url, and version
pass_number = 1
fail_number = 1
for method in valid_methods:
    message_body = request_body(method)
    for url in valid_urls:
        for version in valid_versions:
            # Randomly choose any combination of 1 or more headers
            headers = random.sample(valid_headers, k=random.randint(1, len(valid_headers)))
            write_pass_request(method, url, version, headers, pass_number, message_body)
            pass_number += 1

            # Generate failing tests
            if pass_number % 4 == 0:
                invalid_method = random.choice(invalid_methods)
                write_fail_request(invalid_method, url, version, headers, fail_number, message_body)
                fail_number += 1
            elif pass_number % 4 == 1:
                invalid_url = random.choice(invalid_urls)
                write_fail_request(method, invalid_url, version, headers, fail_number, message_body)
                fail_number += 1
            elif pass_number % 4 == 2:
                invalid_version = random.choice(invalid_versions)
                write_fail_request(method, url, invalid_version, headers, fail_number, message_body)
                fail_number += 1
            else:
                invalid_header = random.choice(invalid_headers)
                headers[len(headers) - 1] = invalid_header # Always replace the last header
                write_fail_request(method, url, version, headers, fail_number, message_body)
                fail_number += 1
            
# Ensure there is a passing and failing test with no header
method = random.choice(valid_methods)
url = random.choice(valid_urls)
version = random.choice(valid_versions)
message_body = request_body(method)
write_pass_request(method, url, version, None, pass_number, message_body)
pass_number += 1
method = random.choice(invalid_methods)
write_fail_request(method, url, version, None, fail_number, message_body)
fail_number += 1

for status_code in valid_status_codes:
    numeric_part = status_code.split()[0]
    message_body = response_body(int(numeric_part[:3]))
    for version in valid_versions:
        # Randomly choose any combination of 1 or more headers
        headers = random.sample(valid_headers, k=random.randint(1, len(valid_headers)))
        write_pass_response(version, status_code, headers, pass_number, message_body)
        pass_number += 1

        if pass_number % 3 == 0:
            invalid_version = random.choice(invalid_versions)
            write_fail_response(invalid_version, status_code, headers, fail_number, message_body)
            fail_number += 1
        elif pass_number % 3 == 1:
            invalid_status_code = random.choice(invalid_status_codes)
            write_fail_response(version, invalid_status_code, headers, fail_number, message_body)
            fail_number += 1
        else:
            invalid_header = random.choice(invalid_headers)
            headers[len(headers) - 1] = invalid_header # Always replace the last header
            write_fail_response(version, status_code, headers, fail_number, message_body)
            fail_number += 1

# Ensure there is a passing and failing test with no header
status_code = random.choice(valid_status_codes)
version = random.choice(valid_versions)
numeric_part = status_code.split()[0]
message_body = response_body(int(numeric_part[:3]))
write_pass_response(version, status_code, None, pass_number, message_body)
pass_number += 1
version = random.choice(invalid_versions)
write_fail_response(version, status_code, None, fail_number, message_body)
fail_number += 1

# Create tests where non-optional fields are omitted - first generate a passing test and then a corresponding failing test
# Omit method in request
method = random.choice(valid_methods)
message_body = request_body(method)
url = random.choice(valid_urls)
version = random.choice(valid_versions)
headers = random.sample(valid_headers, k=random.randint(0, len(valid_headers)))
write_pass_request(method, url, version, headers, pass_number, message_body)
pass_number += 1
method = ""
write_fail_request(method, url, version, headers, fail_number, message_body)
fail_number += 1

# Omit url in request
method = random.choice(valid_methods)
message_body = request_body(method)
url = random.choice(valid_urls)
version = random.choice(valid_versions)
headers = random.sample(valid_headers, k=random.randint(0, len(valid_headers)))
write_pass_request(method, url, version, headers, pass_number, message_body)
pass_number += 1
url = ""
write_fail_request(method, url, version, headers, fail_number, message_body)
fail_number += 1

# Omit version in request
method = random.choice(valid_methods)
message_body = request_body(method)
url = random.choice(valid_urls)
version = random.choice(valid_versions)
headers = random.sample(valid_headers, k=random.randint(0, len(valid_headers)))
write_pass_request(method, url, version, headers, pass_number, message_body)
pass_number += 1
version = ""
write_fail_request(method, url, version, headers, fail_number, message_body)
fail_number += 1

# Omit version in response
version = random.choice(valid_versions)
status_code = random.choice(valid_status_codes)
numeric_part = status_code.split()[0]
message_body = response_body(int(numeric_part[:3]))
headers = random.sample(valid_headers, k=random.randint(0, len(valid_headers)))
write_pass_response(version, status_code, headers, pass_number, message_body)
pass_number += 1
version = ""
write_fail_response(version, status_code, headers, fail_number, message_body)
fail_number += 1

# Omit status code in request
version = random.choice(valid_versions)
status_code = random.choice(valid_status_codes)
numeric_part = status_code.split()[0]
message_body = response_body(int(numeric_part[:3]))
headers = random.sample(valid_headers, k=random.randint(0, len(valid_headers)))
write_pass_response(version, status_code, headers, pass_number, message_body)
pass_number += 1
status_code = ""
write_fail_response(version, status_code, headers, fail_number, message_body)
fail_number += 1
