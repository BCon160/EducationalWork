load("pebbleGraph.sage")
from collections import Counter
from sets import Set

class colPebbleGraph(pebbleGraph):
	  '''
	  An extension to the basic pebble game object that allows for decompositions of graphs into trees
	  
	  N (int) Number of nodes in the subject graph
      k (int) Number of pebbles on each node
      l (int) l+1 is the number of pebbles needed on two nodes in order to add an edge between them
      colours (list<string>) List of colours that the pebbles can take in the pebble game
      
      Creating an instance of this class will provide an empty N node graph with k pebbles on each node.  Each of these k pebbles will have a different colour.
      
      The coloured pebble game runs on the same rules as the basic pebble game however each pebble now has a colour.  As a result, this algorithm can produce a decomposition of a graph into trees as well as providing a rigidity classification of the graph.  As long as the graph is suitable to run a pebble game on this class can find a tree decomposition.  This is achieved through the use of colours on pebbles and special add_edge + find_pebble algorithms.
      
      Public Methods:
      get_pebble_colours()
	  change_labels()
	  use_custom_labels()
	  add_edge(i,j)
	  move_pebble(path)
	  find_pebble(i,j)
	  output()
      
      This class inherits from the pebbleGraph class
	  '''
      #Constructor method for colPebbleGraph
      #N, k, l are the standard inputs for a pebble game
      #colours are a set of labels that are used to make the end result more human readable
      def __init__(self, N, k, l, colours = ["blue", "red", "green", "yellow", "orange", "white", "black", "gray", "purple", "pink"]):
      	  pebbleGraph.__init__(self, N, k, l)
	  self.labels = colours
	  #pebbleCols will track which pebbles are where on the graph. While the algorithm runs it is populated with numbers, the user can then change the output to be the colour labels if they wish
	  self.pebbleCols = []
	  #Put a pebble of each colour(number) on each vertex
	  for i in range(self.N):
	      self.pebbleCols.append([])
	      colNum = 0
	      for j in range(self.k):
	      	  self.pebbleCols[i].append(colNum)
		  colNum += 1

      #An extension to the pebbleGraph add_edge. Technically called the 'canonical add edge', this method will only add edges that minimize the chance of a monochromatic cycle being created later on
      #NB: This function assumes that adding an edge between i and j is a legal move an will not check this
      def add_edge(self, i, j, colour = None):
			'''
			Adds an edge from node (int)i to node (int)j that will not create a cycle
			
			This method over-rides pebbleGraph.add_edge(i,j) but extends sage.graphs.digraph.DiGraph.add_edge(i,j)
			'''
			
          #If node i has no pebbles on it, we cannot add an edge
      	  if self.pebbleList[i] == 0:
	     print("Cannot add an adge from node", i, "to node", j, ", not enough pebbles on node", i, "Try adding an edge from", j, "to", i)
	  #If no colour is specified then we find the most common colour between nodes i & j and then add the edge in that colour
	  elif colour == None:
	     seti = Set(self.pebbleCols[i])
	     setj = Set(self.pebbleCols[j])
	     common = seti & setj
	     if len(common) == 0:
	     	label = Counter(self.pebbleCols[i]).most_common(1)[0][0]
	     else:
	        label = common.pop()
	     sage.graphs.digraph.DiGraph.add_edge(self, i, j, label)
	     self.pebbleCols[i].remove(label)
	     self.pebbleList[i] = self.pebbleList[i] - 1
	  #Otherwise it is a simple case of adding the edge in the specified colour
	  else:
	     sage.graphs.digraph.DiGraph.add_edge(self, i, j, colour)
	     self.pebbleCols[i].remove(colour)
	     self.pebbleList[i] = self.pebbleList[i] - 1

      #Moves a pebble along a set of nodes and a updates edge colours + directions accordingly
      #pathInfo contains a list of vertices(the path) and the colour of pebble that should be moved on each vertex
      def move_pebble(self, pathInfo):
      	  '''
      	  Moves a pebble along a (list<int>) path
      	  
      	  The path leads from where a pebble was found to where it is needed.  This method takes a pebble along that path and updates pebble positions and edge colours along the way. 
      	  
      	  This method over-rides pebbleGraph.move_pebble(path)
      	  '''
      	  
      	  path = pathInfo[0]
	  col = pathInfo[1]
	  for i in range(len(path)-1):
     	      j = i + 1
	      #Move pebble of the edge and onto vertex j
	      self.pebbleCols[path[j]].append(col[path[j]])
	      self.pebbleList[path[j]] += 1
	      #Reverse the edge
	      self.delete_edge(path[j], path[i], col[path[j]])
	      self.add_edge(path[i], path[j], col[path[i]])
	      
      #Method that finds a 'canonical path' to a pebble. This is a path that avoids the creation of a monochromatic cycle in the pebbleGraph
      #Returns a list with a path and the pebble colours corresponding to that path(see the 'colours' dictionary in the code)
      def __find_can_path(self, i, visited):
      	  #Start off with a standard pebbleGame depth first search to find a pebble
      	  path = self._DFS(i,visited)
	  #If the DFS fails then there is no pebble, return this result
	  if path == []:
	     return([path, {}])
	  #Otherwise we have found a path, now we make it into a canonical one
	  else:
	     #Initialise the colours dictionary
	     colours = {}
	     #The reconstruct dictionary will allow us to re-trace the path once it is canonical
	     reconstruct = {}
	     reconstruct[path[0]] = "end"
	     #The colour on the first node is arbitrary
	     colours[path[0]] = self.pebbleCols[path[0]][0]
	     for i in range(len(path)):
	     	 j = i - 1
	     	 #If the colour of node i is already set we don't have to worry about it
	     	 if path[i] in colours:
		    pass
		 #If the colour of node j is the same as the edge ij then ij will not create a monochromatic cycle 
		 elif colours[path[j]] in self.edge_label(path[i], path[j]):
		    #Set i as the next node in the path and set the colour of i
		    reconstruct[path[i]] = path[j]
		    colours[path[i]] = colours[path[j]]
		 #Otherwise using edge ij could create a monochromatic cycle, we have to find a way around this
		 else:
		    #In case we want to return to the previous configuration(this can happen if we have no chioice but to accept edge ij in the path)
		    copcolDict = dict(colours)
		    coprec = dict(reconstruct)
		    #Try to find a new path that doesn't use ij and has the same colour as node j
		    newPath = self.__connect_paths(reconstruct,colours,colours[path[j]],path[i])
		    #If we could not find a good work around we revert to using edge ij
		    if newPath == []:
		       colours = copcolDict
		       reconstruct = coprec
		       colours[path[i]] = self.edge_label(path[i], path[j])[0]
		       reconstruct[path[i]] = path[j]
		    else:
		       pass
	     curNode = path[-1]
	     returnPath = []
	     #Reconstruct the path
	     while curNode != "end":
		   returnPath.append(curNode)
		   curNode = reconstruct[curNode]
	     #The path is reconstructed in reverse so undo this by reversing again
	     returnPath.reverse()
 	     return([returnPath, colours])

      #Exactly the same as pebbleGraph.find_pebble except it uses the find_can_path method rather than the find_path method
      def find_pebble(self, i, j):
      	  '''
      	  Finds a path to a pebble to add to either node (int)i or node (int)j that will not create a cycle in the graph
      	  
      	  This method finds a "canonical" path through the graph to a pebble. This is just a path that creates no monochromatic cycles in the graph 
      	  
      	  This method over-rides pebbleGraph.find_pebble
      	  '''
      	  
      	  visited = [i,j] 
	  if self.pebbleList[i] == self.k:
	     pathInfo = self.__find_can_path(j,visited)
	  elif self.pebbleList[j] == self.k:
	     pathInfo = self.__find_can_path(i,visited)
	  else:
	     pathInfo = self.__find_can_path(i,visited)
	     if pathInfo[0] == []:
	     	pathInfo = self.__find_can_path(j,visited)
	     else:
		pass
	  return(pathInfo)

      #Changes a section of path from being non canonical to canonical
      def __connect_paths(self,reconstruct,colours,colour,startNode):
      	  curNode = startNode
	  #newPath keeps track of the path that we find
	  newPath = [startNode]
	  #nextNode tells us if we can go on with our search for a canonical section
	  nextNode = None
	  #Give nextNode an initial value
	  for u in self.neighbors_out(curNode):
	      if colour in self.edge_label(curNode, u):
	      	 nextNode = u
	      else:
		 pass
	  #If nextNode is None then there is no vertex that we can go to from curNode so we stop. If nextNode is in colours then we may have found a new canonical section
	  while nextNode != None and nextNode not in colours:
	      nextNode = None
	      #Check each out edge of curNode, if it's colour is the one we are searching for then move along that edge, otherwise do nothing
	      for v in self.neighbors_out(curNode):
	      	 if colour in self.edge_label(curNode, v):
		    newPath.append(v)
		    curNode = v
		    nextNode = v
		    break
		 else:
		    pass
	  #If the last node on our new section of path has already been encountered in __find_can_path(if it's in colours) then it may be our answer
	  if newPath[-1] in colours:
	     #If the colour of the last edge matches the last node then we have found our new canonical section
	     if colours[newPath[-1]] in self.edge_label(newPath[-2], newPath[-1]):
	     	#Update the path and colours dictionaries
	        for i in range(len(newPath)-1):
		    j = i + 1
		    reconstruct[newPath[i]] = newPath[j]
		    colours[newPath[i]] = colours[newPath[i-1]]
	     	return(newPath)
	     #Otherwise the new path is still not canonical, we need to go again by recursively calling connect_paths
	     else:
	        morePath = self.__connect_paths(reconstruct,colours,colour,newPath[-2])
		#If the next layer down could not find a path then no path is possible, this is fed back up the line
		if morePath == []:
		   return([])
		#We can update colours and reconstruct and feed the combined newPath and morePath up the line
		else:
		   for i in range(len(newPath)-1):
		       j = i + 1
		       reconstruct[newPath[i]] = newPath[j]
		       colours[newPath[i]] = colours[newPath[j]]
		   #newPath[-1] is the same as morePath[0] so we can exclude morePath[0] in the returned path
		   return(newPath + morePath[1:])
          #Otherwise no new path was found, return this to the layer above
	  else:
	     return([])

      #This method changes all of the edge labels and pebble colours into words rather than numbers. Should only be called when the algorithm has run its course or before the algorithm runs(less efficient)
      def change_labels(self):
      	  '''
      	  Changes the colour labels on edges and labels on pebbles on vertices into strings
      	  
      	  The strings are just text representations of colour such as 'blue', 'green' ect. rather than integers
      	  Note: This should be done after the pebble game has been run since comparing integers is much more efficient than comparing strings.
      	  '''
      	  if self.k > 10:
	     print("Too many pebble colours for the default label set, cannot convert labels (default set has 10 colours in it). Please provide a set of colours by using the use_custom_labels() method")
	     return(None)
	  elif self.allows_multiple_edges() == True:
	     print("Cannot change labels into strings due to multi-edges in graph")
	     return(None)
	  else:
	     pass
      	  if self._can_change_labels() is True:
      	     for e in self.edges():
	      	 i = e[0]
	      	 j = e[1]
	      	 self.set_edge_label(i,j,self.labels[self.edge_label(i,j)[0]])
	     for i in range(len(self.pebbleList)):
		 for j in range(self.pebbleList[i]):
		     self.pebbleCols[i][j] = self.labels[self.pebbleCols[i][j]]
	  else:
	     print("Cannot change labels twice")

      #Returns true if the labels in the pebble graph have already been made into words, false otherwise
      def _can_change_labels(self):
      	  '''
      	  Return true if the labels can be changed to their string forms, false otherwise
      	  
      	  The labels cannot be changed into string form because either: (1) They are already in string form or (20) There are parallel edges in the graph the set_edge_label(i,j) method cannot handle
      	  '''
      	  
      	  edges = self.edges()
	  if edges == []:
	     if type(self.pebbleCols[0][0]) is str:
	     	return(False)
	     else:
	     	return(True)
	  else:
	     edge = edges[0]
	     if type(self.edge_label(edge[0],edge[1])[0]) is str:
	     	return(False)
	     else:
		return(True)

      def output(self):
      	  '''Outputs the rigidity classification of the input graph and the final configuration of the edges'''
      	  pebbleGraph.output(self)
	  print("The final edge layout of the game was:")
	  print(self.edges())

      #Accessor method for pebble colours
      def get_pebble_colours(self):
	  return(self.pebbleCols)

      #Method to use your own colour labels
      def use_custom_labels(self, colours):
      	  '''
      	  Changes from the default labels to labels of the user’s choice
      	  
      	  Useful when dealing with a pebble game with K > 10 (there are only 10 default labels)
      	  '''
      	  self.labels = colours
