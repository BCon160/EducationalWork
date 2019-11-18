class GardenController < ApplicationController
	skip_before_action :authorize
	def index
		@vegetables = Vegetable.order(:name)
	end
end
