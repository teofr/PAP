dp = [-1]*33
dp[0] = 0
dp[1] = 1
dp[2] = 1
dp[3] = 1

for x in range(4, 33):
	dp[x] = dp[x-1] + dp[x-3]


print dp[32]
