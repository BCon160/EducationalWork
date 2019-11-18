class AddAcceptedToExtraTips < ActiveRecord::Migration
  def change
    add_column :extra_tips, :accepted, :boolean
  end
end
