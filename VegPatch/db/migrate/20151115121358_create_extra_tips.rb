class CreateExtraTips < ActiveRecord::Migration
  def change
    create_table :extra_tips do |t|
      t.string :name
      t.string :email
      t.text :tip
      t.references :vegetable, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
