# XML Parser 



## Overview 

Partial working parser for xml files , primarily the gmr.xml files generated for each parser
but also is meant to support general xml documents.


For now can parse an XML declaration and default doctype declaration.

Unfortunately trying to write the grammar for  nested child elements and sequential child elements resulted in many 
parser conflicts so the current parser can only handle very basic cases.

The information used to construct the current parser was not obtained from any one resource but rather several online
ones that include but are not limited to:


https://www.tutorialspoint.com/xml/xml_documents.htm#:~:text=An%20XML%20document%20is%20a,structure%20or%20a%20mathematical%20equation.

https://www.w3schools.com/xml/xml_syntax.asp

https://xmlwriter.net/xml_guide/doctype_declaration.shtml


https://www.w3.org/TR/xml/#sec-well-formed

