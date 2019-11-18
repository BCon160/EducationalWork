class LineItem < ActiveRecord::Base
  belongs_to :vegetable
  belongs_to :allotment
end
