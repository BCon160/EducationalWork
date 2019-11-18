json.array!(@vegetables) do |vegetable|
  json.extract! vegetable, :id, :name, :instructions, :description, :tips, :easiness_to_grow_rating, :image_url
  json.url vegetable_url(vegetable, format: :json)
end
