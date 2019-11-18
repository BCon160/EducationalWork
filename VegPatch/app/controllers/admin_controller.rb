class AdminController < ApplicationController
  def index
	@total_tips = ExtraTip.count
  end
end
