# abnf -- A BNF parser for binary formats.

**abnf** is a Bison parser that extends Bison's BNF to be consistent with <STANDARD>


## Examples

The following examples demonstrate the primary differences between BNF and aBNF.

BNF: <rule> ::= __structure__

aBNF Equivalent: rule =  __structure__

NOTE:

Rule names are case insensitive.

The names <rulename>, <Rulename>, <RULENAME>, and <rUlENamE> all refer to the same rule.

Unlike original BNF, angle brackets ("<", ">") are not required. However, angle brackets may be used around a rule name whenever their presence facilitates in discerning the use of a rule name.

A few, additional notable diffrences can be seen in the following operators:
- Sequence Group:  (Rule1 Rule2)
- Variable Repetition:  *Rule
- Specific Repetition:  nRule
- Optional Sequence:  [RULE]


