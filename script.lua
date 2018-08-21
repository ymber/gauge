local function printstring(string)
    print("called from callprint")
    print(string)
end

function callprint()
    print("calling printstring")
    printstring("test string")
end

function mainfunc()
    callprint()
end
