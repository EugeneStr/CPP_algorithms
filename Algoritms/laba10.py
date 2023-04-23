import random

# генерация комбинации
def GenComb(): 
  C=""
  for i in range(3):
    C+=str(random.randrange(2))
  return C

# реализация самой игры
for i in range(3):
  A=GenComb() # комбинация игрока А
  B=GenComb() # комбинация игрока В
# проверка различия комбинаций игроков
while A==B:
  B=""
  for i in range(3):
    B=GenComb()
print("A =", A)
print("B =", B)
R = ""
# пока кто-то не победит
while A!=R[-3:] and B!=R[-3:]:
  R+=str(random.randrange(2))
  print("R =", R)
if A==R[-3:]:
  print("Победил игрок A")
else:
  print("Победил игрок B")
  
# варианты комбинаций для игроков
A = ['000', '001', '010', '011', '100', '101', '110', '111']
B = ['000', '001', '010', '011', '100', '101', '110', '111']
# количество побед при определенных комбинациях
countA= [[0] * len(A) for i in range(len(B))]
countB= [[0] * len(A) for i in range(len(B))]
# количестов раундов
rounds=1000
for r in range(rounds):
  for i in range(len(A)):
    for j in range(len(B)):
      if i==j: # совпадение комбинаций
        countA[i][j]=rounds/2
        countB[i][j]=rounds/2
        continue
      R = "" # последовательность бросков
      while A[i]!=R[-3:] and B[j]!=R[-3:]: # пока кто-то не победит
        R+=str(random.randrange(2))
      if A[i]==R[-3:]:
        #print("Победил игрок A")
        countA[i][j]+=1
      else:
        #print("Победил игрок B")
        countB[i][j]+=1

sumA=0
print(" "*5 ,end="| ")
for i in range(len(A)):
    print(' %s '%A[i], end="| ")
print()
# средняя вероятность победы игрока А при каждой комбинации
for i in range(len(A)):
  print(' %s '%B[i], end="| ")
  for j in range(len(B)):
    countA[i][j]/=rounds
    sumA+=countA[i][j]
    print('%.3f'%countA[i][j],end="| ")
  print()

# вероятность победы игрока А при определенной комбинации
sA=[0] * len(A) 
print(" "*5 ,end="| ")
for i in range(len(A)):
  for j in range(len(B)):
    sA[i]+=countA[j][i]
  print('%.3f'%(sA[i]/len(A)),end="| ")
print()

# общая вероятность победы игрока А при любой комбинации
print("Вероятность выигрыша игрока А = ",round(sumA/64, 5))
print()

sumB=0
print(" "*5 ,end="| ")
for i in range(len(A)):
    print(' %s '%A[i], end="| ")
print()
# средняя вероятность победы игрока В при каждой комбинации
for i in range(len(A)):
  print(' %s '%B[i], end="| ")
  for j in range(len(B)):
    countB[i][j]/=rounds
    sumB+=countB[i][j]
    print('%.3f'%countB[i][j],end="| ")
  print()

# вероятность победы игрока В при определенной комбинации
sB=[0] * len(B) 
print(" "*5 ,end="| ")
for i in range(len(B)):
  for j in range(len(A)):
    sB[i]+=countB[j][i]
  print('%.3f'%(sB[i]/len(B)),end="| ")
print()

# общая вероятность победы игрока В при любой комбинации
print("Вероятность выигрыша игрока B = ",round(sumB/64, 5))
print()

# Сумма общих вероятностей для проверки подсчета
print("Сумма вероятностей выиграша обоих игроков (должно быть ~1) = ", sumA/64 + sumB/64)
