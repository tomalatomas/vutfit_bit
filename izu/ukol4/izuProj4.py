import numpy

#Trenovaci mnoziny Xp
data = [
    [ 1, 2, 3],
    [ 1, 2, 4],
    [ 4, 2, 1],
    [ 2, 3, 1],
    [-3,-4, 3],
    [ 2,-2, 2],
    [ 3, 4, 1],
    [-3, 2, 1],
    [-2,-2, 0],
    [ 4, 0,-4],
    [-2, 2,-3],
    [ 1,-1,-1]
]

#Prototypy Wj
midpoints = [
    [-2, 2,-4],[ 1,-3,-4],[-3, 4, 1]
]

print("Body:")
for i in range(len(data)):
    print("%d:" % (i+1), data[i])

print()

print("Stredové body:")
for i in range(len(midpoints)):
    print("%d:" % (i+1), midpoints[i])

print()

#Pocet prototypu
number_of_dimensions = 3

groups = []
former_groups = []

for i in range(number_of_dimensions):
    groups.append([])
    former_groups.append([])

iterations = 0

while True:
    print("Iterácia", iterations)
    for i in range(len(data)):
        #Prochazeni bodu
        print("Bod %d:" % (i+1))
        distances = numpy.array(numpy.zeros(number_of_dimensions))
        for j in range(len(midpoints)):
        #Pro kazdy prototyp je vypocitana euklidovska vzdalenost k bodu
            distances[j] = numpy.linalg.norm(numpy.array(data[i]) - numpy.array(midpoints[j]))
            print("  Vzdialenosť od cieľa %d: %f" % (j+1, distances[j]))
        #Nejmensi vzdalenost je pridana do skupiny prototypu
        if distances[0] <= distances[1] and distances[0] <= distances[2]:
            groups[0].append([i, data[i]])
        elif distances[1] <= distances[0] and distances[1] <= distances[2]:
            groups[1].append([i, data[i]])
        elif distances[2] <= distances[0] and distances[2] <= distances[1]:
            groups[2].append([i, data[i]])
    
    for i in range(len(groups)):
        print("Skupina %d:" % (i+1))
        for j in range(len(groups[i])):
            print("  Bod %d:" % (groups[i][j][0] + 1))
            print(" ", groups[i][j][1])

    if groups == former_groups:
        print("Skupiny sa zhodli, výsledné stredy:")
        for point in midpoints:
            print(point)
        break
    else:
        former_groups = groups

    for i in range(len(midpoints)): # set each midpoint
        positions = [.0, .0, .0]
        for j in range(len(groups[i])): # with each midpoint, check it's group
            for k in range(number_of_dimensions): # check each dimension
                positions[k] += groups[i][j][1][k]
        for j in range(number_of_dimensions):
            try:
                midpoints[i][j] = positions[j] / len(groups[i])
            except ZeroDivisionError:
                pass
    
    print("Stredové body:")
    for i in range(len(midpoints)):
        print("%d:" % (i+1), midpoints[i])
    
    groups = []
    for i in range(number_of_dimensions):
        groups.append([])
    
    iterations += 1

