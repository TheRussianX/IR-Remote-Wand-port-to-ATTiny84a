##This script converts a binary number from raw signal data to LSB hex variant of Arduino

while True:
    command_binary_str = input("Enter your binary command: ")
    num_bits_str = input("Enter the number of bits:")
    command_num_of_bits = int(num_bits_str)
    command_binary = bin(int(command_binary_str, 2))[2:].zfill(command_num_of_bits)
    print(command_binary)
    #read the binary command from right to left
    inverted_binary_command = command_binary[::-1] #backwards beacause of LSB
    print(inverted_binary_command)
    #convert the inverted binary command to hex command
    hex_raw_command = hex(int(inverted_binary_command, 2))
    print(hex_raw_command)

