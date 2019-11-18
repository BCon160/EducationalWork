json.array!(@allotments) do |allotment|
  json.extract! allotment, :id
  json.url allotment_url(allotment, format: :json)
end
