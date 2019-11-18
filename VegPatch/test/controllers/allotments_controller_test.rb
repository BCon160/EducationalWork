require 'test_helper'

class AllotmentsControllerTest < ActionController::TestCase
  setup do
    @allotment = allotments(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:all_allotments)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create allotment" do
    assert_difference('Allotment.count') do
      post :create, allotment: {  }
    end

    assert_redirected_to allotment_path(assigns(:allotment))
  end

  test "should show allotment" do
    get :show, id: @allotment
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @allotment
    assert_response :success
  end

  test "should update allotment" do
    patch :update, id: @allotment, allotment: {  }
    assert_redirected_to allotment_path(assigns(:allotment))
  end

  test "should destroy allotment" do
    assert_difference('Allotment.count', -1) do
      delete :destroy, id: @allotment
    end

    assert_redirected_to allotments_path
  end
end
