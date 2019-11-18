class CreateAllotments < ActiveRecord::Migration
  def change
    create_table :allotments do |t|

      t.timestamps null: false
    end
  end
end
