module CurrentAllotment
  extend ActiveSupport::Concern

  private

  def set_cur_allotment 
    @cur_allotment = Allotment.find(session[:allotment_id])
  rescue ActiveRecord::RecordNotFound
	@cur_allotment = Allotment.create
	session[:allotment_id] = @cur_allotment.id
  end
end