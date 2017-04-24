module splitOut(input [13:0] IN, output reg [15:0] OUT);
	always @(IN) begin
		OUT[3:0] = IN % 10;
		OUT[7:4] = IN / 10 % 10;
		OUT[11:8] = IN / 100 % 10;
		OUT[15:12] = IN / 1000 % 10;
	end
endmodule

module splitIn(input [6:0] IN, output reg [7:0] OUT);
	always @(IN) begin
		OUT[3:0] = IN % 10;
		OUT[7:4] = IN / 10;
	end
endmodule

module seg(input [3:0] IN, output reg [7:0] OUT);
	always @(IN) begin
		case(IN)
			4'b0000: OUT <= 8'b11000000;
			4'b0001: OUT <= 8'b11111001;
			4'b0010: OUT <=	8'b10100100;
			4'b0011: OUT <= 8'b10110000;
			4'b0100: OUT <= 8'b10011001;
			4'b0101: OUT <= 8'b10010010;
			4'b0110: OUT <= 8'b10000011;
			4'b0111: OUT <= 8'b11111000;
			4'b1000: OUT <= 8'b10000000;
			4'b1001: OUT <= 8'b10010000;
			default: OUT <= 8'b11111111;
		endcase
	end
endmodule

module alu(input [6:0] a, input [6:0] b, input [1:0] op, output reg [13:0] out);
	always @(a or b or op) begin
		case(op)
			2'b00: out = a + b;
			2'b01: out = a - b;
			2'b10: out = a * b;
			2'b11: out = a / b;
		endcase		
	end
endmodule

module main(input [17:0] iSW, output [7:0] oHEX7_D,oHEX6_D,oHEX5_D,oHEX4_D,oHEX3_D,oHEX2_D,oHEX1_D,oHEX0_D);
	
	wire [13:0] out;


	alu alu1(iSW[17:11], iSW[10:4], iSW[1:0], out);


	wire [7:0] aSplited;
	wire [7:0] bSplited;
	wire [15:0] outSplited;
	
	splitIn split1(iSW[17:11], aSplited);
	splitIn split2(iSW[10:4], bSplited);

	seg seg1(aSplited[7:4],oHEX7_D);
	seg seg2(aSplited[3:0],oHEX6_D);
	seg seg3(bSplited[7:4],oHEX5_D);
	seg seg4(bSplited[3:0],oHEX4_D);

	splitOut split3(out, outSplited);

	seg seg5(outSplited[15:12],oHEX3_D);
	seg seg6(outSplited[11:8],oHEX2_D);
	seg seg7(outSplited[7:4],oHEX1_D);
	seg seg8(outSplited[3:0],oHEX0_D);

endmodule

