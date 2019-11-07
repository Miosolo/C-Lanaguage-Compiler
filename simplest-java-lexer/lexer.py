# read the file
CODE_PATH = 'sample.java'
with open(CODE_PATH) as f:
  codelines = f.readlines()

# rules
keywords = ['abstract', 'assert', 'boolean', 'break', 'byte',
            'case', 'catch', 'char', 'class', 'const', 'continue',
            'default', 'do', 'double', 'else', 'enum', 'extends',
            'final', 'finally', 'float', 'for', 'goto', 'if',
            'implements', 'import', 'instanceof', 'int', 'interface',
            'long', 'native', 'new', 'package', 'private', 'protected',
            'public', 'return', 'strictfp', 'short', 'static', 'super',
            'switch', 'synchronized', 'this', 'throw', 'throws',
            'transient', 'try', 'void', 'volatile', 'while']
symbols = ['+', '-', '*', '/', '%', '++', '--',
           '==', '!=', '>', '<', '>=', '<=',
           '&', '|', '^', '~', '<<', '>>', '>>>',
           '&&', '||', '!',
           '=', '+=', '-=', '*=', '/=', '&=', '<<=', '>>=', '&=', '^=', '|=',
           '{', '}', '(', ')', '[', ']', '"', '"', ';', '.', ',']
# todo: 3-tuple, instanceof
idchars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
           'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
           'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
           'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 
           'W', 'X', 'Y', 'Z','_'}
numchars = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}
symchars = {'!', '"', '%', '&', "'", '(', ')', '*', '+', '-', '/', '<', '=', ',',
            '>', '^', '{', '|', '}', '~', '{', '}', '(', ')', '"', '"', ';', '.', '[', ']',}
blankchars = {' ', '\t', '\n'}


def stateful_parse(state, c):
  ''' (state, char) -> (tuple(or None), next_state)'''
  if state == 'start':
    if c in idchars:
      return None, 'id'
    elif c in numchars:
      return None, 'int'
    elif c in symchars - {'"'}:
      return None, 'sym'
    elif c == '"':
      return None, 'str'
    elif c in blankchars:
      return None, 'start'
    else:
      raise RuntimeError('Undefined sequence')
  elif state == 'id':
    if c in idchars | numchars:
      return None, 'id'
    elif c in blankchars:
      return (parsed, 'keyword' if parsed in keywords else 'identifier'), 'start'
    elif c in symchars:
      return (parsed, 'keyword' if parsed in keywords else 'identifier'), 'sym'
    else:
      raise RuntimeError('Undefined sequence')
  elif state == 'int':
    if c in numchars:
      return None, 'int'
    elif c == '.':
      return None, 'float'
    elif c in symchars:
      return (parsed, 'number'), 'sym'
    elif c in blankchars:
      return (parsed, 'number'), 'start'
    else:
      raise RuntimeError('Undefined sequence')
  elif state == 'float':
    if c in numchars:
      return None, 'float'
    elif c in symchars:
      return (parsed, 'number'), 'sym'
    elif c in blankchars:
      return (parsed, 'number'), 'start'
    else:
      raise RuntimeError('Undefined sequence')
  elif state == 'sym':
    #comment
    if parsed+c == '//':
      return None, 'comment'
    # greedy
    elif parsed+c in symbols:
      return None, 'sym'
    elif c == '"':
      return (parsed, 'symbol'), 'str'
    elif c in symbols:
      return (parsed, 'symbol'), 'sym'
    elif c in blankchars:
      return (parsed, 'symbol'), 'start'
    elif c in idchars:
      return (parsed, 'symbol'), 'id'
    elif c in numchars:
      return (parsed, 'symbol'), 'int'
    else:
      raise RuntimeError('Undefined sequence')
  elif state == 'str':
    if c != '"':
      return None, 'str'
    else:
      return (parsed+'"', 'string'), 'start'
  else:
    raise RuntimeError('Undefined sequence')


# tuple: (position, class, value)
tokens = []
for l in range(len(codelines)):
  line = codelines[l]
  parsed = ''
  state = 'start'

  for i in range(len(line)):
    token, state = stateful_parse(state, line[i])
    if token is not None:
      tokens.append(((l+1, i), *token))
      parsed = ''
    if state == 'comment':
      parsed = ''
      state = 'start'
      break
    if state != 'start':
      parsed += line[i]

for t in tokens:
  print(t)