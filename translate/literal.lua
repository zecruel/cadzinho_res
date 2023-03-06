files = {}
literals = {}

pfile = io.popen('ls -a ../src')
for lin in pfile:lines() do
  if string.find(lin, '^gui.*%.c$') then
    files[#files + 1] = lin
  end
end

out = io.open('en_GB.lua', 'w+')
out:write ('translate = {}\n')

for _, file in ipairs(files) do
  out:write('--------- ' .. file .. ' --------- \n')
  for lin in io.lines('../src/' .. file) do
    for w in string.gmatch(lin, '_l%("(.+)"%)') do
      if not literals[w] then
        out:write('translate["' ..  w .. '"] = "' .. w .. '"\n')
        literals[w] = true
      end
    end
  end
end

out:close()