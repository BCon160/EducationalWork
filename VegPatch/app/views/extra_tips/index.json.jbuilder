json.array!(@extra_tips) do |extra_tip|
  json.extract! extra_tip, :id, :name, :email, :tip, :vegetable_id
  json.url extra_tip_url(extra_tip, format: :json)
end
