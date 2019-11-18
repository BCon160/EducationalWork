class CreateLineItems < ActiveRecord::Migration
  def change
    create_table :line_items do |t|
      t.references :vegetable, index: true, foreign_key: true
      t.references :allotment, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
