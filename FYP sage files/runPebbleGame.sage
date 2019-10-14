load("compPebbleGraph.sage")
load("colPebbleGame.sage")

def runPebbleGame(G,k=2,l=3):
    P = pebbleGraph(len(G.vertices()),k,l)
    for e in G.edges():
    	i = e[0]
	j = e[1]
	layout = P.get_pebble_layout()
	if (P.can_add_edge(i,j)) and layout[i] == 0:
	   i = e[1]
	   j = e[0]
	else:
	   pass
	while not (P.can_add_edge(i,j)):
	      pathToPebb = P.find_pebble(i,j)
	      if pathToPebb == []:
		 break
	      else:
		 P.move_pebble(pathToPebb)
	if (P.can_add_edge(i,j)):
	      if layout[i] > 0:
	      	 P.add_edge(i,j)
	      else:
		 P.add_edge(j,i)
	else:
	      P.reject_edge(i,j)

    return(P.output()) 
	
def runCompGame(G,k=2,l=3):
    P = componentGraph(len(G.vertices()),k,l)
    for e in G.edges():
    	i = e[0]
	j = e[1]
	if (P.can_add_edge(i,j)) and P.pebbleList[i] == 0:
	   i = e[1]
	   j = e[0]
	else:
	   pass
	if l == 0:
	   component = P.get_components()
	   if (i == j and (l >= k and l < 2*k)) or (component[i] == 1 and component[j] == 1):
	      P.reject_edge(i,j)
	   else:
	      analyse_edge(i,j,P)
	elif l <= k:
	   components = P.get_components()
	   if (i == j and (l >= k and l < 2*k)) or (components[i] == components[j] and components[i] != -1):
	      P.reject_edge(i,j)
	   else:
	      analyse_edge(i,j,P)
	else:
	   compMat = P.get_component_matrix()
	   if (i == j and (l >= k and l < 2*k)) or compMat[i][j] == 1:
	      P.reject_edge(i,j)
	   else:
	      analyse_edge(i,j,P)

    print(P.edges())
    return(P.output())

def analyse_edge(i,j,PG):
    while not (PG.can_add_edge(i,j)):
	  pathToPebb = PG.find_pebble(i,j)
	  PG.move_pebble(pathToPebb)
    PG.add_edge(i,j)
    newComp = PG.detect_component(i,j)
    if len(newComp) > 0:
	  PG.update_components(newComp)
    else:
	  pass

def runColGame(G, k=2, l=3):
    P = colPebbleGraph(len(G.vertices()),k,l)
    for e in G.edges():
    	i = e[0]
	j = e[1]
	if (P.can_add_edge(i,j)) and P.pebbleList[i] == 0:
	   i = e[1]
	   j = e[0]
	else:
	   pass  
	while not (P.can_add_edge(i,j)):
	      pathInfo = P.find_pebble(i,j)
	      if pathInfo[0] == []:
		 break
	      else:
		 P.move_pebble(pathInfo)
	if (P.can_add_edge(i,j)):
	      P.add_edge(i,j)
	else:
	      P.reject_edge(i,j)
    if not P.allows_multiple_edges():
       P.change_labels()
    else:
       pass
    return(P)
