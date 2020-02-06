/**
Descripcion,
Modulo que sincroniza las senales (hsync y vsync)
de un controlador VGA de 640x480 60hz, funciona con un reloj de 25Mhz

Ademas tiene las coordenadas de los pixeles H (eje x),
y de los pixeles V (eje y). Para enviar la senal RGB correspondiente
a cada pixel

-----------------------------------------------------------------------------
Author : Nicolas Hasbun, nhasbun@gmail.com
File   : vgaHdmi.v
Create : 2017-06-15 15:07:05
Editor : sublime text3, tab size (2)
-----------------------------------------------------------------------------
*/

// **Info Source**
// https://eewiki.net/pages/viewpage.action?pageId=15925278

module vgaHdmi(
  // **input**
  input clock, clock50, reset,
  input switchR, switchG, switchB,

  // **output**
  output reg hsync, vsync,
  output reg dataEnable,
  output wire vgaClock,
  output [23:0] RGBchannel
);

reg [9:0]pixelH, pixelV; // estado interno de pixeles del modulo
reg vgaClock_s;

initial begin
  hsync      = 1;
  vsync      = 1;
  pixelH     = 0;
  pixelV     = 0;
  dataEnable = 0;
  vgaClock_s   = 0;
end

// Manejo de Pixeles y Sincronizacion

always @(posedge clock or posedge reset) begin
  if(reset) begin
    hsync  <= 1;
    vsync  <= 1;
    pixelH <= 0;
    pixelV <= 0;
  end
  else begin
    // Display Horizontal
    if(pixelH==0 && pixelV!=1124) begin
      pixelH<=pixelH+1'b1;
      pixelV<=pixelV+1'b1;
    end
    else if(pixelH==0 && pixelV==1124) begin
      pixelH <= pixelH + 1'b1;
      pixelV <= 0; // pixel 525
    end
    else if(pixelH<=1920) pixelH <= pixelH + 1'b1;
    // Front Porch
    else if(pixelH<=2008) pixelH <= pixelH + 1'b1;
    // Sync Pulse
    else if(pixelH<=2052) begin
      pixelH <= pixelH + 1'b1;
      hsync  <= 0;
    end
    // Back Porch
    else if(pixelH<2199) begin
      pixelH <= pixelH+1'b1;
      hsync  <= 1;
    end
    else pixelH<=0; // pixel 800

    // Manejo Senal Vertical
    // Sync Pulse
    if(pixelV == 1085 || pixelV == 1086 || pixelV == 1087 || pixelV == 1088 || pixelV == 1089)
      vsync <= 0;
    else
      vsync <= 1;
  end
end

// dataEnable signal
always @(posedge clock or posedge reset) begin
  if(reset) dataEnable<= 0;

  else begin
    if(pixelH >= 0 && pixelH <640 && pixelV >= 0 && pixelV < 480)
      dataEnable <= 1;
    else
      dataEnable <= 0;
  end
end

// VGA pixeClock signal
// Los clocks no deben manejar salidas directas, se debe usar un truco

initial vgaClock_s = 0;

always @(posedge clock50 or posedge reset) begin
  if(reset) vgaClock_s <= 0;
  else      vgaClock_s <= ~vgaClock_s;
end

assign vgaClock = vgaClock_s;

// **************************************************************
// Screen colors using de10nano switches for test

//assign RGBchannel[23:16] = (switchR)? 8'd255 : 8'd0;
//assign RGBchannel [15:8] = (switchG)? 8'd255 : 8'd0;
//assign RGBchannel  [7:0] = (switchB)? 8'd255 : 8'd0;


wire [1:0] sel;

assign sel[0] = switchB;  // 
assign sel[1] = switchG;  // 

wire [36:0] RGBchannel_s;



assign RGBchannel_s[35:24] = (sel == 2'b01)? 12'hfff : 12'd0;  // 
assign RGBchannel_s[23:12] = (sel == 2'b10)? 12'hfff : 12'd0;  // 
assign RGBchannel_s[11:0]  = (sel == 2'b11)? 12'hfff : 12'd0;  // 

ddr18 (
	.aclr(1'b0),
	.outclock(vgaClock_s),
	.datain_l(RGBchannel_s[35:18]),
   .datain_h(RGBchannel_s[17:0]),
   .dataout (RGBchannel  [17:0]),
);
/*

assign RGBchannel[23:16] = 8'd255;
assign RGBchannel [15:8] = 8'd255;
assign RGBchannel  [7:0] = 8'd255;
*/



endmodule