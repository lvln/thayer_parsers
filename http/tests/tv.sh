#!/bin/bash
rm -f ./pass.* ./fail.* 

# [PASSING TESTS]
echo -en "GET /where?q=now HTTP/1.1\r\n\r\nMessage\r\n" > ./pass.1
echo -en "PUT * HTTP/1.0\r\n\r\n" > ./pass.2
echo -en "TRACE http://www.example.org/pub/WWW/TheProject.html HTTP/1.2\r\nHost: localhost8000\r\n\r\nMessage\r\n" > ./pass.3
echo -en "CONNECT www.example.com:80 HTTP/1.1\r\nHost:loalhost:8000\r\nHost: localhost:8000\r\n\r\n" > ./pass.4
echo -en "GET / HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept-Language: fr\r\n\r\n" > ./pass.5
echo -en "GET /background.png HTTP/1.0\r\nImage: requested\r\n\r\nMessage" > ./pass.6
echo -en "POST /contact_form.php HTTP/1.1\r\nHost: developer.mozilla.org\r\nContent-Length: 64\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nname=Joe%20User&request=Send%20me%20one%20of%20your%20catalogue" > ./pass.7
echo -en "OPTIONS /anypage.html HTTP/1.0\r\nHost: localhost\r\nAuthority: me\r\n\r\n" > ./pass.8
echo -en "GET https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages HTTP/1.1\r\nLocal-host: localhost:8000\r\n\r\nError with newline\r\n" > ./pass.9
echo -en "CONNECT developer.mozilla.org:80 HTTP/1.1\r\nRobust: tests\r\n\r\nMulti-worded message with a special character ^ and no newline following it" > ./pass.10
echo -en "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\r\n<!DOCTYPE html>\r\n<html lang="en">" > ./pass.11
echo -en "HTTP/1.1 100 Continue\r\n\r\n" > ./pass.12
echo -en "HTTP/1.1 201 Created\r\nHost: server side\r\n\r\nMessage\r\n" > ./pass.13
echo -en "HTTP/1.1 303 Moved permanently\r\nServer: Apache/2.4.37 (Red Hat)\r\nontent-Type: text/html; charset=utf-8\r\nDate: Thu, 06 Dec 2018 17:33:08 GMT\r\n\r\nThis is a message\r\n" > ./pass.14
echo -en "HTTP/1.1 503 Service Unavailable\r\n\r\n<!DOCTYPE html>… (contains a site-customized page helping the user to find the missing resource)" > ./pass.15
echo -en "CONNECT www.example.com:80 HTTP/1.1\r\nNo-field-value:\r\n\r\n" > ./pass.16
echo -en "CONNECT www.example.com:80 HTTP/1.1\r\nField-name:Multi-lined field\r\n value\r\n\r\n" > ./pass.17

# [FAILING TESTS]
echo -en "GET /where?q=now HTTP/1.1\r\nMessage\r\n" > ./fail.1
echo -en "PUT * HTTP/1.0\r\nHost:host\r\nMessage\r\n" > ./fail.2
echo -en "TRANE http://www.example.org/pub/WWW/TheProject.html HTTP/1.2\r\nHost: localhost:8000\r\n\r\nMessage\r\n" > ./fail.3
echo -en "CONNECT www.example.com:80 HTTP/1.11\r\nHost:loalhost 43\r\n\r\n" > ./fail.4
echo -en "GET / HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept-Language fr\r\n\r\n" > ./fail.5
echo -en "GET ~ HTTP/1.1\r\nHost: developer.mozilla.org\r\nAccept-Language: fr\r\n\r\n" > ./fail.6
echo -en "POST /contact_form.php HTT/1.1\r\nHost: developer.mozilla.org\r\nContent-Length: 64\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nname=Joe%20User&request=Send%20me%20one%20of%20your%20catalogue" > ./fail.7
echo -en "OPTIONS /anypage.html HTTP/1.0\r\nHost: localhost\r\nAuthority: me my\r\n" > ./fail.8
echo -en "https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages HTTP/1.1\r\nReduce: hostName\r\n\r\nError\r\n" > ./fail.9
echo -en "CONNECT developer.mozilla.org:80\r\nRobust: tests\r\n\r\nThis is a message" > ./fail.10
echo -en "CONNECT  HTTP/1.1\r\nRobust: tests\r\n\r\nThis is a message" > ./fail.11
echo -en "HTTP/12.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\r\n<!DOCTYPE html>\r\n<html lang="en">" > ./fail.12
echo -en "HTTP/1.1 600 Continue\r\n\r\n" > ./fail.13
echo -en "HTTP/1.1 500 Continue\r\n" > ./fail.14
echo -en "HTTP/1.1 5004 Service Unavailable\r\n\r\n<!DOCTYPE html>… (contains a site-customized page helping the user to find the missing resource)" > ./fail.15
echo -en "HTTP/1.1 100 \r\n\r\n" > ./fail.16
echo -en "HTTP/1.1 Continue\r\n\r\n" > ./fail.17
echo -en "HTTP/1.1  Continue\r\n\r\n" > ./fail.18
echo -en "GET /where?q=now \r\nMessage\r\n" > ./fail.19
echo -en " /where?q=now HTTP/1.1\r\nMessage\r\n" > ./fail.20
echo -en "GET HTTP/1.1\r\nMessage\r\n" > ./fail.21
echo -en "CONNECT www.example.com:80 HTTP/1.1\r\nField-name:Multi-lined field\r\nvalue with no space\r\n\r\n" > ./fail.22

# generate a file with the number of pass and fail tests
#NUMPASS=(pass.*)
#NUMFAIL=(fail.*)
#echo "${#NUMPASS[@]} ${#NUMFAIL[@]}" > ./numtests
