class ExtraTip < ActiveRecord::Base
  belongs_to :vegetable
  validates :name, :email, :tip, :vegetable_id, presence: true
  validates :tip, uniqueness: true
end
