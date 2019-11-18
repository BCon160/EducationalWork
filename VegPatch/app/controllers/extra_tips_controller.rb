class ExtraTipsController < ApplicationController
  skip_before_action :authorize, only: [:new, :create]
  before_action :set_extra_tip, only: [:show, :edit, :update, :destroy]

  # GET /extra_tips
  # GET /extra_tips.json
  def index
    @extra_tips = ExtraTip.all
  end

  # GET /extra_tips/1
  # GET /extra_tips/1.json
  def show
  end

  # GET /extra_tips/new
  def new
    @extra_tip = ExtraTip.new
  end

  # GET /extra_tips/1/edit
  def edit
  end

  # POST /extra_tips
  # POST /extra_tips.json
  def create
    @extra_tip = ExtraTip.new(extra_tip_params)

    respond_to do |format|
      if @extra_tip.save
        format.html { redirect_to garden_path, notice: 'Tip was successfully created.  It will be processed soon.' }
        format.json { render :show, status: :created, location: @extra_tip }
      else
        format.html { render :new }
        format.json { render json: @extra_tip.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /extra_tips/1
  # PATCH/PUT /extra_tips/1.json
  def update
    respond_to do |format|
      if @extra_tip.update(extra_tip_params)
        format.html { redirect_to @extra_tip, notice: 'Extra tip was successfully updated.' }
        format.json { render :show, status: :ok, location: @extra_tip }
      else
        format.html { render :edit }
        format.json { render json: @extra_tip.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /extra_tips/1
  # DELETE /extra_tips/1.json
  def destroy
    @extra_tip.destroy
    respond_to do |format|
      format.html { redirect_to extra_tips_url, notice: 'Tip was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_extra_tip
      @extra_tip = ExtraTip.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def extra_tip_params
      params.require(:extra_tip).permit(:name, :email, :tip, :vegetable_id, :accepted)
    end
end
