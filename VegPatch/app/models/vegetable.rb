class Vegetable < ActiveRecord::Base
	before_destroy :ensure_not_referenced_by_any_line_item
	has_many :line_items
	before_destroy :ensure_not_referenced_by_any_extra_tip
	has_many :extra_tips, dependent: :destroy
	validates :name, :instructions, :description, :image_url, presence: true
	validates :name, uniqueness: true
	validates :name, length: {minimum: 2}
	validates :easiness_to_grow_rating, numericality: {greater_than_or_equal_to: 0}
	validates :easiness_to_grow_rating, numericality: {less_than_or_equal_to: 10}
	def to_s
		name
	end
	
	
	private

	# ensure that there are no line items referencing this product
	def ensure_not_referenced_by_any_line_item
		if line_items.empty?
			return true
		else
			errors.add(:base, 'Line Items present')
			return false
		end
	end
	
	private

	# ensure that there are no extra tips referencing this product
	def ensure_not_referenced_by_any_extra_tip
		if extra_tips.empty?
			return true
		else
			errors.add(:base, 'Tips witing to be added present')
			return false
		end
	end
end
