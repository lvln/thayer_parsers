""" file: combinator.py
    brief: A sequence combinator for the push-down automaton.
    by: gunnar pope <gunnar@bitstory.ai>
    date: 1/9/23
    usage: $ python3 combinator.py gmr.xml
    output: gmr.cmb.xml
"""
import xml.etree.ElementTree as ET
import json
import pdb
tree = ET.parse('gmr.xml')
root = tree.getroot()

# create a new rule for the range
def create_new_range_rules():
  """ Replace the first element in a range with the new range token."""

  for rg in franges.keys():
    new_rule = franges[rg]['old_rules'].pop(0) # the first element becomes the new rule
    franges[rg]['new_rule'] = new_rule         # all pointers to old rules need to map to this rule
    print(new_rule)


    for rule in rules.findall('rule'):
      if rule.get('number') == new_rule:
        rule.find('rhs').find('symbol').text = rg


def get_range_rules(first: int, last: int)->list:  
  """ Get a list of all the old rules covering range between first and last."""
  matching_rules = []

  # iterate over all the rules and find the matching symbol to first
  for rule in rules.findall('rule'):
    # print(f"Rulenum: {rule.get('number')}")
    rhs = rule.find('rhs')
    symbol = rhs.find('symbol').text.strip("'")
    # print(f"symbol: {symbol}, len: {len(symbol)}")
    # pdb.set_trace()

    # print('symbol: ', type(symbol))
    if len(symbol) == 1:
      if ord(symbol) >= first and ord(symbol) <= last: # 97 = int('a')
        print('Match: ', symbol)
        matching_rules.append(rule.get('number'))

  return matching_rules
def sym2int(sym):
  return int(format(ord(sym)))

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

def print_ranges():
  print(json.dumps(ranges, indent=4))

for rule in rule_list:
  print(f"Rulenum: {rule.get('number')}")
  lhs = rule.find('lhs')
  rang = lhs.text
  if 'r__' in rang:
    if rang not in ranges.keys():
      ranges[rang] = {}
      ranges[rang]['symbols'] = []
      ranges[rang]['rules'] = []
      ranges[rang]['in_range'] = []

    ranges[rang]['rules'].append(rule.get('number'))
    rhs = rule.find('rhs')
    symbols = rhs.findall('symbol')
    for symbol in symbols:
      print(f"symbol: {symbol}")
      print(f"symbol.text: {symbol.text}")
      ranges[rang]['symbols'].append(symbol.text)
      ranges[rang]['in_range'].append(1) # these get set to 0 if not withing continuous range 


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
  hasTransitions = False
  hasReduction   = False

  print('state: ', state)
  itemset = state.findall('itemset')
  print('state.itemset: ', itemset)

  print()
  statenum = state.get('number')
  print('State: ', statenum)

  transitions = state[1][0]
  reductions  = state[1][2]

  if (len(transitions) > 0 and 
      len(reductions) > 0 ):
      print('Keep state')

      # remove the terminal from the used list
      ## find the reduction['rule']
      ## if rule in ranges, marke as unused.

      keep_rule = reductions[0].get('rule')
      print('Keep rule', keep_rule)

      for rkey in ranges.keys():
        for (i, rule) in enumerate(ranges[rkey]['rules']):
          if rule == keep_rule:
            ranges[rkey]['in_range'][i] = 0
      # matches = [i for i, x in enumerate(ranges[range]['rules']) if x == keep_rule] 
      # print('Matches: ', matches)

  print('Ranges: ', ranges)

  # for tran in state[1][0]:
  #   print('transion: ', tran)

  
  # for reductions in state[1][2]:
  #   print('reductions: ', reductions)


# for each range, find the continuous symbols to remove 
for r in ranges.keys():
  diff = [0] # the backwards difference

  for i in range(len(ranges[r]['in_range']))[:-1]:
    diff.append(ranges[r]['in_range'][i+1] - ranges[r]['in_range'][i]) 

  print('diff: ', diff)
  consec = 0
  curr_range = 0
  ranges[r]['final_ranges'] = []
  for i in range(len(diff)):
    if diff[i] >= 0: 
      consec += 1
      if consec == 2:
        curr_range += 1
        ranges[r]['final_ranges'][i-1] = curr_range
        ranges[r]['final_ranges'].append(curr_range)
      else:
        ranges[r]['final_ranges'].append(curr_range)
    else:
      consec = 0
      ranges[r]['final_ranges'].append(0)

print('Ranges: ', ranges)

for r in ranges.keys():
  rmax = max(ranges[r]['final_ranges'])

  ranges[r]['subranges'] = {}
  print('subrange count: ', rmax)
  if (rmax >= 1):
    x = 1
    print('Subranges:') 
    while(x <= rmax):
      ranges[r]['subranges'][f"sr__{x}"] = []

      for (i, val) in enumerate (ranges[r]['final_ranges']):
        if val == x:
          print('val', val)
          ranges[r]['subranges'][f"sr__{x}"].append(ranges[r]['symbols'][i])

      x += 1


print_ranges()

# now, remove the unused terminals
terminals = root[1].find('terminals') 
terminals_to_remove = []
for term in terminals:
  for r in ranges:
    for key in ranges[r]['subranges'].keys():
      tname = term.get('name')
      # print(tname)
      # print(ranges[r]['subranges'][key])
      if tname in ranges[r]['subranges'][key]:
        # print('Removing: ', tname)
        terminals_to_remove.append(term) # note: don't remove terminals within the loop!

# remove the terminals outside of the iterator
for term in terminals_to_remove:
  terminals.remove(term)

for term in terminals:
  print('term: ', term.get('name'))

# determine the final set of ranges for the combinator 
new_tnames = []
franges = {} # final ranges
range_token_num = 500

for r in ranges.keys():
  for sub in ranges[r]['subranges'].keys():
    low = ranges[r]['subranges'][sub][0]
    replace_symbol = low
    high= ranges[r]['subranges'][sub][-1]
    low = int(format(ord(low[1:-1]))) # turn "'a'" into int 97
    high = int(format(ord(high[1:-1])))
    rname = f"r_{low}_{high}"
    print(f"{sub}={rname}")

    franges[rname] = {}
    franges[rname]['symbol-number'] = str(range_token_num) # arbitrary for now. will be reassigned later
    franges[rname]['token-number'] = str(range_token_num)
    franges[rname]['name'] = rname
    franges[rname]['usefuleness'] = 'useful' 
    franges[rname]['range'] = [int(low), int(high)]
    range_token_num += 1

print('The final ranges:')
print(franges)

# add in new terminals for the ranges

terminals = root[1].find('terminals') 
for rang in franges.keys():
  ET.SubElement(terminals, 'terminal', franges[rang])

ET.dump(terminals)
# ET.dump(rules)

tree.write('gmr.combinator.xml') 

####################################

# find all 'old' rules related to a given range 
for key in franges.keys():
  replace = True
  print(key)

  # find the starting value of the range.
  first = franges[key]['range'][0]
  last  = franges[key]['range'][1]
  franges[key]['old_rules'] = get_range_rules(first, last)

print('franges: ', franges)




create_new_range_rules()
# ET.dump(rules)

print('franges: ', franges)
# def remove_old_rules():

# def map_old_rules_to_new_rules():

# def renumber_rules():




# map pointers to old rules to new rule

# remove unused rules




#   # pdb.set_trace()
#   if symbol == 'a':
#     print('match "a"')

# remove the unused states

# create a unique terminal for each subrange

# print out a module in C for the pda combinator

# 

if __name__ == "__main__":
  print('The Sequence Combinator')
