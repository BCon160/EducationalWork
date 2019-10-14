load("pebbleGraph.sage")
from sets import Set

class componentGraph(pebbleGraph):
      '''
	  This is an extension to the basic pebble game in which the algorithm keeps track of rigid components as it runs the pebble game.
	  
	  N (int) Number of nodes in the subject graph
      k (int) Number of pebbles on each node
      l (int) l+1 is the number of pebbles needed on two nodes in order to add an edge between them
	  
	  Creating an instance of this class will provide an empty N node graph with k pebbles on each node.
	  
	  The component pebble game runs on the same rules as the basic pebble game except that now, rigid components are kept track of.  Not only does this give a better breakdown of results once the pebble game has run, it also makes for a more efficient algorithm.
	  
	  Public Methods:
		get_components()
		get_component_matrix()
		detect_component(i,j)
		update_components(newComponent)
		attempt_add_edge(i,j)
		output()
	   
	  This class inherits from the pebbleGraph class
	  '''

      def __init__(self, N, k, l):
      	  #Initialise all class variables from the pebbleGraph class
      	  super(componentGraph,self).__init__(N, k, l)
	  #For more info on component tracking schemes se update_component()
	  #When l=0 there is only ever 1 components so we employ a simple component tracking scheme
	  if self.l == 0:
	     self.component = []
	     for i in range(self.N):
	         self.component.append(0)
	  #When l<=k there can be many components but they are non-overlapping, again we use a simple tracking scheme
	  elif self.l <= self.k:
	     self.components = []
	     self.componentNum = 0
	     #Initialise all vertices to be in component -1 (i.e. no component)
	     for i in range(self.N):
	         self.components.append(-1)
	  #Otherwise we can have many components that may overlap at a single vertex
	  else:
	     self.components = []
	     #for efficiency we store a matrix that allows us to look up if 2 vertices are in the same component
	     self.compMat = []
	     #Initialise all matrix entries to 0
	     for i in range(self.N):
	     	 self.compMat.append([])
	     	 for j in range(self.N):
	     	     self.compMat[i].append(0)

      #This method takes a vertex(v) and calculates which vertices can be visited by follwing edges away from v
      def _reach(self, v, reverse=False):
      	  D = DiGraph(data = self.adjacency_matrix())
      	  #Sometimes we need to search the pebbleGraph in reverse
	  if reverse == True:
	     D = D.reverse()
	  else:
	     pass
	  #Reachable is a set because we need to perform set operations om it down the line
      	  reachable = Set([v])
	  #s will be our 'to visit' stack
	  s = D.neighbors_out(v)
	  #Essentially we perform a depth first search and return the visted nodes
	  while len(s) > 0:
	      u  = s.pop()
	      reachable.add(u)
	      for i in D.neighbors_out(u):
	      	  if i not in reachable:
		     s.append(i)
		  else:
		     pass
	  return(reachable)

      #This method takes two vertices(i,j) and figures out whether the newly added edge between them changes the component structure of the graph
      def detect_component(self, i, j):
	      '''
		  Checks for the creation of a rigid component due to addition of edge (int)i and (int)j
		  
		  This method will return any component found or will return an empty list if none is found
		  '''
      	  #If this condition is met then the edge is free and doesnt change component structure
      	  if self.pebbleList[i] + self.pebbleList[j] > self.l:
	     return([])
	  else:
	     reachij = self._reach(i) | self._reach(j)
	     total = 0
	     #This loop computes the total number of pebbles on reachij
	     for w in reachij:
	     	 total += self.pebbleList[w]
	     #If this total is > l then component structure is unchanged
	     if total > self.l:
	     	 return([])
	     else:
		 #Otherwise we have changed the component structure and this code picks out this new component
		 V = Set(self.vertices())
		 #Set theory: V\reachij
		 V = V - reachij
		 visited = Set()
		 #V is the set of vertices that cannot be reached by following edges from i or j
		 for w in V:
		     #We only perform the limited Depth first search on vertices with a free pebble
		     if self.pebbleList[w] > 0:
	     		temp = self._reach(w,True)
			#Set theory: visited UNION temp
		     	visited = visited | temp
		     else:
		        pass
		 V = Set(self.vertices())
		 #V\visited is the set of vertices that forms the new component
		 return(V - visited)

      #This method takes a newly formed component and does the necessary maintainance of the component tracking structures
      def update_components(self, newComponent):
	      '''
		  Performs maintenance on the component tracking structures of the component graph
		  
		  newComponent (list<int>) List of the elements of a newly formed component
		  '''
      	  #The main condition statements are explained in the constructor
      	  if self.l == 0:
	     for v in newComponent:
	     	 #If a vertex is in the component it is marked with a 1. All vertices not in the component remain as 0
	     	 self.component[v] = 1
	  elif self.l <= self.k:
	     for v in newComponent:
	     	 #Each vertex is given a label corresponding to which component it occurs in. Those vertices with label -1 are free(no component)
	     	 self.components[v] = self.componentNum
	     self.componentNum += 1
	  else:
	     #components is a list of components. Each component is represented by the vertices that are present in it
	     #compMat stores info on whether 2 vertices are in the same component. compMat[i][j] = 1 if i and j are in the same component, 0 otherwise
	     toRemove = []
	     #This loop works out which old components are simply subsets of the new component. Such components can be forgotten about
	     for oldComponent in self.components:
	     	 if oldComponent <= newComponent:
		    toRemove.append(oldComponent)
		 else:
		    pass
	     for oldComponent in toRemove:
	     	 self.components.remove(oldComponent)
	     self.components.append(newComponent)
	     #Update the compMat
	     for i in newComponent:
	     	 for j in newComponent:
		     self.compMat[i][j] = 1
		     self.compMat[j][i] = 1

      #This method is an addon to the pebbleGraph output method and simply outputs information on the components to the user
      def output(self):
	      '''
		  Looks at the final graph structure and layout in order to give results
		  
		  Gives a classification of the Graph and the set of component(s) with an explanation of the component structure.  Note: Only gives correct output when all edges on original graph have been either added to or rejected from the componentGraph.
		  This method is an extension of pebbleGraph.output()
		  '''
      	  super(componentGraph,self).output()
	  print("The component(s) in your graph are: ")
	  if self.l == 0:
	     print(self.component)
	     print("All vertices marked with a 1 are in the component. All vertices marked with a 0 are not in the component") 
	  elif self.l <= self.k:
	     print(self.components)
	     print("All vertices marked with the same number are in the same component. The particular number present is not meaningful in this context. A vertex marked with -1 is in no components.")
	  else:
	     #Print the components in list rather than set form
	     betterComp = []
	     for comp in self.components:
	     	 betterComp.append(list(comp))
	     print(betterComp)

      #This method attempts to add an edge ij by using the component structure of the pebble graph
      def attempt_add_edge(self,i,j):
	    '''
		Performs an edge addition without the user having to call the required methods
		
		If edge ij can be added to the pebble graph, this method performs all necessary pebble movements in order to add the edge then adds the edge and performs necessary component maintenance, otherwise rejects the edge
		'''
		layout = self.get_pebble_layout()
		#This clause makes sure we never fail to add an edge just because all L+1 pebbles are on node j
	      	if (P.can_add_edge(i,j)) and layout[i] == 0:
			temp = i
			i = j
			j = temp
		else:
			pass
		#Component structure is different depending on k and L, the 3 cases are provided for below
		if self.l == 0:
			component = self.get_components()
			#These are the conditions for rejecting an edge
			if (i == j and (self.l >= self.k and self.l < 2*self.k)) or (component[i] == 1 and component[j] == 1):
				self.reject_edge(i,j)
			else:
				self.analyse_edge(i,j)
		elif self.l <= self.k:
			components = self.get_components()
			#These are the conditions for rejecting an edge
			if (i == j and (self.l >= self.k and self.l < 2*self.k)) or (components[i] == components[j] and components[i] != -1):
				self.reject_edge(i,j)
			else:
				self.analyse_edge(i,j)
		else:
			compMat = self.get_component_matrix()
			#These are the conditions for rejecting an edge
			if (i == j and (self.l >= self.k and self.l < 2*self.k)) or compMat[i][j] == 1:
				self.reject_edge(i,j)
			else:
				self.analyse_edge(i,j)

      #This method collects pebbles to add an edge, adds the edge and calls the component maintainance functions
      def analyse_edge(self,i,j):
		  while not (self.can_add_edge(i,j)):
			pathToPebb = self.find_pebble(i,j)
			self.move_pebble(pathToPebb)
		  self.add_edge(i,j)
		  newComp = self.detect_component(i,j)
		  if len(newComp) > 0:
			self.update_components(newComp)
		  else:
			pass
	
      def get_components(self):
		'''
		Returns a list of rigid component(s) present in the graph
		
		Returns a List of some sort whose structure is dependent on k and l.
		If l=0, there is only 1 component, all vertices marked with 1 are in that component, everything else is not.
		If L<K, Components cannot overlap so the entry at each node is the component that that node belongs to. -1 if not in a component.
		Otherwise, the components can overlap so return value is a list of vertex sets each of which is a ridgid component.
		'''
      	  if self.l == 0:
	     return(self.component)
	  elif self.l <= self.k:
	     return(self.components)
	  else:
	     #Return the components in list rather than set form
	     betterComp = []
	     for comp in self.components:
	     	 betterComp.append(list(comp))
	     return(betterComp)

      def get_component_matrix(self):
	  '''
	  Returns a matrix that tells us which vertices are in the same component.
	  
	  If K<L, vertices don't overlap so an empty list is returned
	  '''
	  if self.l > self.k:
	     return(self.compMat)
	  else:
	     print("CompMat is not available for your choices of k and l")
	     return([])