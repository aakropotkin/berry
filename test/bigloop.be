c = clock()
i = 0
while (i < 10000000)
    i = i + 1
end
print('while iteration 10000000 times', clock() - c, 's')

c = clock()
for (i : 1 .. 1000000)
end
print('for   iteration 1000000  times', clock() - c, 's')
# while iteration 10000000 times 0.942453 s
# for   iteration 1000000  times 0.929293 s
