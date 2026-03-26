`timescale 1ns / 1ps
module MultiplicadorVerilog (
    input clk,
    input [31:0] a,
    input [31:0] b,
    output reg [31:0] resultado
);
    always @(posedge clk) begin
        resultado <= a * b;
    end
endmodule