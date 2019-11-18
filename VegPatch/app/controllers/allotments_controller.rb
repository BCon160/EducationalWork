class AllotmentsController < ApplicationController
  skip_before_action :authorize, only: [:show, :create, :update, :destroy]
  rescue_from ActiveRecord::RecordNotFound, with: :invalid_allotment
  before_action :set_allotment, only: [:show, :edit, :update, :destroy]

  # GET /allotments
  # GET /allotments.json
  def index
    @all_allotments = Allotment.all
  end

  # GET /allotments/1
  # GET /allotments/1.json
  def show
  end

  # GET /allotments/new
  def new
    @allotment = Allotment.new
  end

  # GET /allotments/1/edit
  def edit
  end

  # POST /allotments
  # POST /allotments.json
  def create
    @allotment = Allotment.new(allotment_params)

    respond_to do |format|
      if @allotment.save
        format.html { redirect_to @allotment, notice: 'Allotment was successfully created.' }
        format.json { render :show, status: :created, location: @allotment }
      else
        format.html { render :new }
        format.json { render json: @allotment.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /allotments/1
  # PATCH/PUT /allotments/1.json
  def update
    respond_to do |format|
      if @allotment.update(allotment_params)
        format.html { redirect_to @allotment, notice: 'Allotment was successfully updated.' }
        format.json { render :show, status: :ok, location: @allotment }
      else
        format.html { render :edit }
        format.json { render json: @allotment.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /allotments/1
  # DELETE /allotments/1.json
  def destroy
    @allotment.destroy
    respond_to do |format|
      format.html { redirect_to allotments_url, notice: 'Allotment was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    def invalid_allotment
		logger.error "Attempt to access invalid allotment #{params[:id]}"
		redirect_to garden_url, notice: 'Invalid allotment'
	end
    # Use callbacks to share common setup or constraints between actions.
    def set_allotment
      @allotment = Allotment.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def allotment_params
      params[:allotment]
    end
end
