require 'test_helper'

class VegetableTest < ActiveSupport::TestCase
	test "vegetable attributes must not be empty" do
		vegetable = Vegetable.new
		assert vegetable.invalid?
		assert vegetable.errors[:name].any?
		assert vegetable.errors[:instructions].any?
		assert vegetable.errors[:description].any?
		assert vegetable.errors[:easiness_to_grow_rating].any?
		assert vegetable.errors[:image_url].any?
	end
	test "vegetable easiness_to_grow_rating must be between 0 and 10" do
		vegetable = vegetables(:one)

		vegetable.easiness_to_grow_rating = -1
		assert vegetable.invalid?
		assert vegetable.errors[:easiness_to_grow_rating].any?

		vegetable.easiness_to_grow_rating = 11
		assert vegetable.invalid?
		assert vegetable.errors[:easiness_to_grow_rating].any?

		vegetable.easiness_to_grow_rating = 1
		assert vegetable.valid?
		assert vegetable.errors[:easiness_to_grow_rating].none?
	end
end
