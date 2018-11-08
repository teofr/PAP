import random

words = []
last = 0
while len(words) < 10**5:
	last += 1
	words_t = [""]
	for x in range(last):
		words_t_2 = []
		for e in words_t:
			words_t_2.append( "{}0".format(e))
			words_t_2.append( "{}1".format(e))
		words_t = words_t_2
	words = words + words_t

n = 10**5
words_total = 10**5
parags = 5*10**5
parag_size = parags // (n // 20)
# print (parag_size)

def parag():
	p = []
	for x in range(parag_size//2):
		p.append(1)
		p.append(0)
	random.shuffle(p)
	ans = ""
	for x in p:
		ans+=str(x)
	return ans
print(1)
print(n//2)
for x in range(n//20):
	for i in range(20):
		nxt = x*20 + i
		print("+" + words[nxt])
	print("?" + parag())