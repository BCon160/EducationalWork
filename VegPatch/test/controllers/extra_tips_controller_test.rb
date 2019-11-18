require 'test_helper'

class ExtraTipsControllerTest < ActionController::TestCase
  setup do
    @extra_tip = extra_tips(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:extra_tips)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create extra_tip" do
    assert_difference('ExtraTip.count') do
      post :create, extra_tip: { email: @extra_tip.email, name: @extra_tip.name, tip: 'cool cabbage', vegetable_id: @extra_tip.vegetable_id }
    end

    assert_redirected_to garden_path
  end

  test "should show extra_tip" do
    get :show, id: @extra_tip
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @extra_tip
    assert_response :success
  end

  test "should update extra_tip" do
    patch :update, id: @extra_tip, extra_tip: { email: @extra_tip.email, name: @extra_tip.name, tip: @extra_tip.tip, vegetable_id: @extra_tip.vegetable_id }
    assert_redirected_to extra_tip_path(assigns(:extra_tip))
  end

  test "should destroy extra_tip" do
    assert_difference('ExtraTip.count', -1) do
      delete :destroy, id: @extra_tip
    end

    assert_redirected_to extra_tips_path
  end
end
