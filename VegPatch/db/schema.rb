# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20151124204123) do

  create_table "allotments", force: :cascade do |t|
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "extra_tips", force: :cascade do |t|
    t.string   "name"
    t.string   "email"
    t.text     "tip"
    t.integer  "vegetable_id"
    t.datetime "created_at",   null: false
    t.datetime "updated_at",   null: false
    t.string   "accepted"
  end

  add_index "extra_tips", ["vegetable_id"], name: "index_extra_tips_on_vegetable_id"

  create_table "line_items", force: :cascade do |t|
    t.integer  "vegetable_id"
    t.integer  "allotment_id"
    t.datetime "created_at",   null: false
    t.datetime "updated_at",   null: false
  end

  add_index "line_items", ["allotment_id"], name: "index_line_items_on_allotment_id"
  add_index "line_items", ["vegetable_id"], name: "index_line_items_on_vegetable_id"

  create_table "users", force: :cascade do |t|
    t.string   "name"
    t.string   "password_digest"
    t.datetime "created_at",      null: false
    t.datetime "updated_at",      null: false
  end

  create_table "vegetables", force: :cascade do |t|
    t.string   "name"
    t.text     "instructions"
    t.text     "description"
    t.text     "tips"
    t.decimal  "easiness_to_grow_rating"
    t.datetime "created_at",              null: false
    t.datetime "updated_at",              null: false
    t.string   "image_url"
    t.string   "string"
  end

end
