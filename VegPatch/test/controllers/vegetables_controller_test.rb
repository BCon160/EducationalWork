require 'test_helper'

class VegetablesControllerTest < ActionController::TestCase
  setup do
    @vegetable = vegetables(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:vegetables)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create vegetable" do
    assert_difference('Vegetable.count') do
      post :create, vegetable: { description: @vegetable.description, easiness_to_grow_rating: @vegetable.easiness_to_grow_rating, image_url: @vegetable.image_url, instructions: @vegetable.instructions, name: "New Veg", tips: @vegetable.tips }
    end

    assert_redirected_to vegetable_path(assigns(:vegetable))
  end

  test "should show vegetable" do
    get :show, id: @vegetable
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @vegetable
    assert_response :success
  end

  test "should update vegetable" do
    patch :update, id: @vegetable, vegetable: { description: @vegetable.description, easiness_to_grow_rating: @vegetable.easiness_to_grow_rating, image_url: @vegetable.image_url, instructions: @vegetable.instructions, name: @vegetable.name, tips: @vegetable.tips }
    assert_redirected_to vegetable_path(assigns(:vegetable))
  end

  test "should destroy vegetable" do
    assert_difference('Vegetable.count', -1) do
      delete :destroy, id: @vegetable
    end

    assert_redirected_to vegetables_path
  end
  test "vegetable is not valid without a unique name" do
		vegetable = Vegetable.new(	:name  => vegetables(:validtest1).name,
									:instructions 	=> "xxx",
									:description 	=> "xxx", 
									:easiness_to_grow_rating => 1, 
									:image_url   	=> "barney.gif")

		assert vegetable.invalid?
		assert_equal ["has already been taken"], vegetable.errors[:name]
	end
	test "vegetable is not valid without a name of at least 2 characters" do
		vegetable = Vegetable.new(	:name  => "b",
									:instructions 	=> "xxx",
									:description 	=> "xxx", 
									:easiness_to_grow_rating => 1, 
									:image_url   	=> "barney.gif")

		assert vegetable.invalid?
		assert_equal ["is too short (minimum is 2 characters)"], vegetable.errors[:name]
	end
end
