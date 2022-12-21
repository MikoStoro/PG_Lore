module Main where   
import Prelude


countElements :: Eq e => [e] -> e -> Int -> Int
countElements lst element index
    | index >= length lst = 0
    | lst !! index == element = (countElements lst element (index+1))+1
    | otherwise = countElements lst element (index+1)


removeDuplicates :: (Eq a) => [a] -> [a]
removeDuplicates (x:xs) = x : removeDuplicates (filter (/= x) xs)
removeDuplicates [] = []

countInList :: Eq e => [e] -> [e] -> Int -> [(e, Int)]
countInList lst fullLst index   
    | index >= length lst = []
    | otherwise = (lst !! index, countElements fullLst (lst !! index) 0) : countInList lst fullLst (index+1)

countInList' :: Eq e => [e] -> [(e, Int)]
countInList' lst = countInList (removeDuplicates lst) lst 0

main :: IO ()
main = do print (countInList' [1,2,3,3,3,5,5,5,5,5,6,7])
