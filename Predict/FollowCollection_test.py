import FollowCollection
import FirstCollection

def test_follow():
  V_T = {'+', '*', '(', ')', 'eps', 'i'}
  V_N = {'E', 'E\'', 'F', 'T', 'T\''}
  prodDict = {
    'E': [['T', 'E\'']],
    'E\'': [['+', 'T', 'E\''], ['eps']],
    'T': [['F', 'T\'']],
    'T\'': [['*', 'F', 'T\''], ['eps']],
    'F': [['(', 'E', ')'], ['i']]
  }

  expectFollow = {
    'E': {')', '#'},
    'E\'': {')', '#'},
    'T': {'+', ')', '#'},
    'T\'': {'+', ')', '#'},
    'F': {'*', '+', ')', '#'}
  }

  first = FirstCollection.getFirstCollection(prodDict, V_N, V_T)
  actualFollow = FollowCollection.getFollowCollection(prodDict, V_N, V_T, first)
  assert actualFollow == expectFollow