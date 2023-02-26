# SID328 CONVERTER
# Tobias Holm   2023-02-26
#
# Input: a SID-dump of the wanted song.
# Output: a .h file to include in the Arcade game project!

regs = 25 # How many registers to output. 25=including volume. 21=Only voices.
name = "Arkanoid01"
output_file = open("C:/Users/Administrator/myGIT/vga_arcade/Arcade-SID/lib/sid-arduino-lib-master/"+name+".h", "w") # C:\Users\Administrator\myGIT\vga_arcade\Arcade-SID\lib\sid-arduino-lib-master
print('Converting...')
output_file.write('//TH:Converted SID music\n')
output_file.write('const unsigned char sidData'+name+'[] PROGMEM = {\n')

with open(name+'.txt', 'r') as f:
    n=0 # Line number to process
    m=0 # 1=Found +-------+ in source file
    k=0 # Size of written data structure
    t=0 # No tail on first line
    variables = [] # Clear variables
    for line in f:
        n=n+1
        if m==1: # Skip until header read
#            variables = parse_line(line)
            old = variables
            variables = [
# SID-REGISTERS 0-6:  FreqLO        FreqHI        PW LO         PW HI         Cntrl         A/D           S/R
# SIDDump:            Freq                        WF                                        ADSR                      Pul
# SIDDump SPLIT:      FreqHI--------FreqLO        PulHI---------PulLO         WF            AD            SR
                line[ 12: 14],line[ 10: 12],line[ 33: 35],line[ 32: 33],line[ 24: 26],line[ 27: 29],line[ 29: 31],
                line[ 40: 42],line[ 38: 40],line[ 61: 63],line[ 60: 61],line[ 52: 54],line[ 55: 57],line[ 57: 59],
                line[ 68: 70],line[ 66: 68],line[ 89: 91],line[ 88: 89],line[ 80: 82],line[ 83: 85],line[ 85: 87],
                line[ 94: 96],line[ 96: 98],line[ 99:101],line[106:107]
            ]
            # Reuse old values if no change
            for i in range(0, len(variables)):
                if '.' in variables[i]:
                    #print(variables[i],'a-',old[i])
                    variables[i] = old[i]
                    #print(variables[i],'b-',old[i])
            #s=s.replace(".", "0")
                    
            # Mute Channel 1
            for i in range(0, 7):
                pass
#                variables[i] = "00"
                
            # Mute Channel 2
            for i in range(7, 14):
                pass
#                variables[i] = "00"

            # Mute Channel 3
            for i in range(14, 21):
                pass
#                variables[i] = "00"

            # Set same volume on entire song
#            variables[24] = "0" # Volume has NO effect in NanoSID play routine

            # Write data
            s="Only 25 or 21 regs supported!"
            if regs==25:
                s="0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{}".format(*variables)
            if regs==21:
                s="0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{},0x{}".format(*variables)
                
            if t==1:
                output_file.write(',\n')
            t=1 # Tail on the rest of lines (except the last line!)
            output_file.write(s)
            k=k+regs
            if n<15:
                print(n,line,s)
#            output_file.write("0x{},0x{},0x0{},0x{},0x{},0x{},0x{},0x{},0x{},0x0{},0x{},0x{},0x{},0x{},0x{},0x{},0x0{},0x{},0x{},0x{},0x{},\n".format(*variables))

#             output_file.write("{} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}\n".format(
#                 var1, var2, var3, var4, var5, var6, var7, var8, var9, var10,
#                 var11, var12, var13, var14, var15, var16, var17, var18, var19, var20,
#                 var21, var22))
        if '+-------+' in line:
            m=1

output_file.write('};\n')
output_file.write('const int sidLength'+name+' = ' + str(k-1) + ';  //number of bytes in array\n')
output_file.close() # Close the output file
f.close()
print('Done! sidLength =',k-1)
print('Calculted fill of Uno-memory: ',(k+3590)/32256)
