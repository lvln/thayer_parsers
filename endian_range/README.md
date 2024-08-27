
An xbnf parser that accpets ranges on endian primitives for fixed-width integers:

* `big_endian(<integer>, <integer>, <type>)`
* `little_endian(<integer>, <integer>, <type>)`
where `<type>` is either `int16`, `int32`, `int64`, `uint16`, `uint32`, or `uint64`
