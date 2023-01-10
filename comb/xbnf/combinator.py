""" file: combinator.py
    brief: A sequence combinator for the push-down automaton.
    by: gunnar pope <gunnar@bitstory.ai>
    date: 1/9/23
    usage: $ python3 combinator.py gmr.xml
    output: gmr.cmb.xml
"""
import xml.etree.ElementTree as ET
import pdb
tree = ET.parse('gmr.xml')
root = tree.getroot()



# Discovery
print(f"len(root): {len(root)}")
# 3

## bison-xml-report
print(root[0])

## grammar 
print(root[1])
### rules
print(root[1][0])

#### rule
print(root[1][0][0])

##### rule['number'] or rule['usefulnes']
print(root[1][0][0].keys())

rules = root[1].find('rules') 
print(rules)

for rule in rules.iter(): 
  """
    rules
    rule
    lhs
    rhs
    symbol
    symbol
    rule
    ...
  """
  print(rule.tag)

  if rule.find('number') == 0:
    print('Rule 0!')
    print(rule['rhs'])

rule_list = rules.findall('rule')
print(rule_list)

# find the ranges in the grammar
ranges = {}

for rule in rule_list:
  print(f"Rulenum: {rule.get('number')}")
  lhs = rule.find('lhs')
  rang = lhs.text
  if 'r__' in rang:
    if rang not in ranges.keys():
      ranges[rang] = {}
      ranges[rang]['symbols'] = []
      ranges[rang]['rules'] = []
      ranges[rang]['used'] = []

    ranges[rang]['rules'].append(rule.get('number'))
    rhs = rule.find('rhs')
    symbols = rhs.findall('symbol')
    for symbol in symbols:
      print(f"symbol: {symbol}")
      print(f"symbol.text: {symbol.text}")
      ranges[rang]['symbols'].append(symbol.text)


  rhs = rule.find('rhs')
  print(f"lhs: {lhs}")
  print(f"lhs.text: {lhs.text}")
  print(f"rhs: {rhs}")
  symbols = rhs.findall('symbol')
  for symbol in symbols:
    print(f"symbol: {symbol}")
    print(f"symbol.text: {symbol.text}")

# Step 1: Discover all the ranges
print(f"Ranges:  {ranges}")

# Step 2: Iterate over the states. 
# Determine which states have:
#    1. a transition with type="shift"
#    2. a reduction with a rule equal to a ranges.rang.rules 
# If found, mark as unused in range

automaton = root[2]
for state in automaton.findall('state'):
  print('state: ', state)
  itemset = state.findall('itemset')
  print('state.itemset: ', itemset)

  # for actions in state.findall('actions'):
  #   for trans in actions.findall('transitions'):
  #     for tran in trans.find('transition'):

  #       print('transion: ', tran)

  print()
  statenum = state.get('number')
  print('State: ', statenum)

  for tran in state[1][0]:
    print('transion: ', tran)

  
  for reductions in state[1][2]:
    print('reductions: ', reductions)

## automaton
print()
print("Automaton Section: ", root[2])

## grammar
#### rule.number & .usefulness
##### lhs
##### rhs
## automaton

# for rule in root[1].find('rules').iter():
#   rulenum = rule.get('number')
#   if rulenum is not None: 
#     print(f"Rulenumber: {rulenum}")




# pdb.set_trace()
for child in root:
  print(child.tag, child.attrib) 

if __name__ == "__main__":
  print('The Sequence Combinator')
