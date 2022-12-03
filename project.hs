

module Main where   
import Data.List
import Prelude

splitNum :: Integer -> [Integer]
splitNum 0 = []
splitNum n = (n `mod` 10):(splitNum(n `div` 10))

mergeInt :: [Integer] -> Integer
mergeInt [] = 0
mergeInt x = head x + 10 * mergeInt(drop 1 x)

isPrime :: Integer -> Bool
isPrime k = 
    if k < 3 then False
    else null [x | x<-[2 .. floor(sqrt(fromIntegral k))], k `mod` x == 0]

createIntList :: Integer -> [Integer]
createIntList n = nub [ mergeInt x | x<- permutations(splitNum n)]

projectFunction :: Integer -> [Integer]
projectFunction n = [x | x<-[2 .. n], isPrime x == True ]


main :: IO ()
main = do print ("Enter Pascal's row number")
          n <- readLn
          print (pascal (n :: Integer))
