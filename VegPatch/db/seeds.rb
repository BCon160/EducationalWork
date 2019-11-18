# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)
Vegetable.delete_all

Vegetable.create!(name: 'Carrot',
	instructions:
	%{<p>
		Sow thickly out in the soil March to May.  Harvest thinnings at 
		first.  Full grown carrots will be available to pick from
		August onwards.
	</p>},
	description:
	%{<p>
		This umbelliferae is Native to Ireland.  No veg garden is complete 
		without it.  Growing your own carrots is well worth the effort
		because anything you grow will be far superior to any shop bought 
		variety.
	</p>},
	tips:
	%{<p>
		Grows best in a fine soil (no stones).  Try to plant at least 6 
		inches above ground level to make sure the carrot fly can't
		find your carrots.
	</p>},
	image_url: 'carrot.jpg',
	easiness_to_grow_rating: 9)

Vegetable.create!(name: 'Cabbage',
	instructions:
	%{<p>
		Sow thickly out in the soil April to May.  Transplant all the 
		best plants to a different bed thinly.  Full grown cabbages will 
		be available to pick from September onwards.
	</p>},
	description:
	%{<p>
		This brassica is Native to Ireland.  Cabbage is a healthy leaf 
		vegetable full of iron and other important vitamins.
	</p>},
	tips:
	%{<p>
		Grows best in a well compacted soil.  Remember to put slug 
		protection around your plants otherwise all of the greenest,
		most succulent leaves will be severely damaged.
	</p>},
	image_url: 'cabbage.JPG',
	easiness_to_grow_rating: 7)
	
Vegetable.create!(name: 'Potato',
	instructions:
	%{<p>
		Leave seed potatoes in the sun to chit before planting roughly 40cm
		apart in late March. Spray regularly for blight. Potatoes should be
		available from August onwards. Harvest when foliage dies away.
	</p>},
	description:
	%{<p>
		Native to North America, potatoes are one of the easiest sources of 
		carbohydrate to grow in your back garden. They are also a good source
		of potassium.
	</p>},
	tips:
	%{<p>
		Pour soil around the base of stems of plants to improve yields. Whenever
		the weather becomes humid blight will thrive. At the first sign of blight
		pull up the offending plant and burn it.
	</p>},
	image_url: 'potato.jpg',
	easiness_to_grow_rating: 6)
	
Vegetable.create!(name: 'Peas',
	instructions:
	%{<p>
		Plant individual peas out in April about 15cm apart(row wise) but 
		keep the rows well apart(100/150cm). Pick regularly when pods become fat. 
		The first harvest should be available around mid-June and will continue 
		to be available until September weather permitting.
	</p>},
	description:
	%{<p>
		Sweet and tasty these peas are full of vitamins and nutrients. All
		peas you grow yourself will be better than any shop bought varieties.
	</p>},
	tips:
	%{<p>
		Keep harvesting the pods otherwise the plants will stop producing new
		flowers. Watch out for hidden pods in the middle of the plants.
	</p>},
	image_url: 'peas.jpg',
	easiness_to_grow_rating: 9)
	
User.delete_all

User.create!(name: 'benny', password: 'password')