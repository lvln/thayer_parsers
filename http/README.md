# Grammar for HTTP messages defined in xbnf

## Message format:
__HTTP__ messages have the basic format:
* A *start line* describing the message being sent or received
* Optional set of *headers* which contain a field and a value
* A blank line indicating that all meta information has been sent
* An optional *message body*

Messages may be of 2 types:
* __Requests__: The *start line*, or *request line* contains the *HTTP method*, the *request target* and the *protocol version*
  * *HTTP methods* may be:
	  * *GET*
	  * *HEAD*
	  * *POST*
	  * *PUT*
	  * *DELETE*
	  * *CONNECT*
	  * *OPTIONS*
	  * *TRACE*
* __Responses__: The *start line* or *status line* contains the *protocol version*, a 3 digit *status code* and a desription of the status code calles the *status text*
  * *status codes* are 3 digit numbers where the first 2 digits define the class of response:
	* *1xx*: Informational
	* *2xx*: Successful
	* *3xx*: Redirection
	* *4xx*: Client Error
	* *5xx*: Server Error

Headers have the following format:
* The header *field name* which is a case insensitive string
* A colon following the *field name*
* A header *field value*
* Note: the whole header will appear on a single line as line folds are now obsolete

## References:
* [HTTP Dcumentation](https://developer.mozilla.org/en-US/docs/Web/HTTP)
* [RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing](https://datatracker.ietf.org/doc/html/rfc7230)
