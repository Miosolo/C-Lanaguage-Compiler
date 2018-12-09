import PredictParsing

def test_predictTable():
  V_T = {'+', '*', '(', ')', 'eps', 'i', '#'}
  V_N = {'E', 'E\'', 'F', 'T', 'T\''}
  prodDict = {
    'E': [['T', 'E\'']],
    'E\'': [['+', 'T', 'E\''], ['eps']],
    'T': [['F', 'T\'']],
    'T\'': [['*', 'F', 'T\''], ['eps']],
    'F': [['(', 'E', ')'], ['i']]
  }

  expectTable = {
    'E': {
      'i': ['T', 'E\''],
      '(': ['T', 'E\''],
    },
    'E\'': {
      '+': ['+', 'T', 'E\''],
      ')': ['eps'],
      '#': ['eps'],
    },
    'T': {
      'i': ['F', 'T\''],
      '(': ['F', 'T\''],
    },
    'T\'': {
      '+': ['eps'],
      '*': ['*', 'F', 'T\''],
      ')': ['eps'],
      '#': ['eps'],
    },
    'F': {
      'i': ['i'],
      '(': ['(', 'E', ')'],
    }
  }

  actualTable = PredictParsing.getPredictTable(prodDict, V_T, V_N)
  assert expectTable == actualTable