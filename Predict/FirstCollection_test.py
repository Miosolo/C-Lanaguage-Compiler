import FirstCollection

def test_first():
  V_T = {'+', '*', '(', ')', 'eps', 'i'}
  V_N = {'E', 'E\'', 'F', 'T', 'T\''}
  prodDict = {
    'E': [['T', 'E\'']],
    'E\'': [['+', 'T', 'E\''], ['eps']],
    'T': [['F', 'T\'']],
    'T\'': [['*', 'F', 'T\''], ['eps']],
    'F': [['(', 'E', ')'], ['i']]
  }

  expectFIRST = {
    'E': {'(', 'i'},
    'E\'': {'+', 'eps'},
    'T': {'(', 'i'},
    'T\'': {'*', 'eps'},
    'F': {'(', 'i'}
  }

  actualFIRST = FirstCollection.getFirstCollection(prodDict, V_N, V_T)
  assert actualFIRST == expectFIRST