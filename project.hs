

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
createIntList n = [ mergeInt x | x<- rotations(splitNum n)]

spinArray :: [Integer] -> [Integer]
spinArray intArr = drop 1 (intArr ++ [(head intArr)])

rotationHelper :: Int -> [Integer] -> [[Integer]]
rotationHelper n intArr = 
    if n == 0 then []
    else ((rotationHelper (n-1) (spinArray intArr)) ++ [(spinArray intArr)])

rotations :: [Integer] -> [[Integer]]
rotations intArr = rotationHelper (length intArr) intArr

checkNumber :: Integer -> Bool
checkNumber n = null[ x | x <- createIntList(n), isPrime x == False] 


checkNumberTest :: Integer -> [Integer]
checkNumberTest n = [ x | x <- createIntList(n), isPrime x] 

projectFunction :: Integer -> [Integer]
projectFunction n = [x | x<-[1 .. n], checkNumber x]


main :: IO ()
main = do print ("Enter Pascal's row number")
          n <- readLn
          print ((n :: Integer))
