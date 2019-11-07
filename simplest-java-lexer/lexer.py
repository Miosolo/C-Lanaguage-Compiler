# %%
# read the file
CODE_PATH = 'simplest-java-lexer/sample.java'
with open(CODE_PATH) as f:
  codelines = f.readlines()

# %%
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
delimiters = ['{', '}', '(', ')', '"', '"']
symbols = ['+', '-', '*', '/', '%', '++', '--',
           '==', '!=', '>', '<', '>=', '<=',
           '&', '|', '^', '~', '<<', '>>', '>>>',
           '&&', '||', '!',
           '=', '+=', '-=', '*=', '/=', '&=', '<<=', '>>=', '&=', '^=', '|=']
# todo: 3-tuple, instanceof
idchars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
           'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
           'y', 'z', '_'}
numchars = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}
symchars = {'!', '"', '%', '&', "'", '(', ')', '*', '+', '-', '/', '<', '=',
            '>', '^', '{', '|', '}', '~'}
blankchars = {' ', '\t'}

# %%
def stateful_parse(state, c):
  ''' (state, char) -> (tuple(or None), next_state)'''
  if state == 'start':
    if c in idchars:
      return None, 'id'
    elif c in numchars:
      return None, 'num'
    elif c in symchars - {'"'}:
      return None, 'sym'
    elif c == '"':
      return None, 'str'
    elif c in blankchars:
      return None, 'start'
    else:
      raise RuntimeError
  elif state == 'id':
    if c in idchars + numchars:
      return None, 'id'
    elif c in blankchars:
      return (position, parsed, 'keyword' if parsed in keywords else 'identifier'), 'start'
    elif c in symchars:
      return (position, parsed, 'keyword' if parsed in keywords else 'identifier'), 'sym'
    else:
      raise RuntimeError
    

# %%
# tuple: (position, class, value)
tokens = []
for l in range(len(codelines)):
  line = codelines[l].strip()
  curword = ''
  breakflag = False

  for i in range(len(line)):
    # firstly, try to parse an identifier
    c = l[i]

