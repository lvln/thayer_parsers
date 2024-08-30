
An xbnf parser that accpets two endian primitives for fixed-width integers:

* `\<type\>(\<integer\>)` is an integer in network byte order (big-endian)
* `little_endian(<integer>, <type>)` is an integer in little-endian representation
where `<type>` is either `int16`, `int32`, `int64`, `uint16`, `uint32`, or `uint64` for both with the addition of `int8` and `uint8` for network byte order
