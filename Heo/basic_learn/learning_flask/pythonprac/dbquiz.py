from pymongo import MongoClient
client = MongoClient('localhost', 27017)
db = client.dbsparta


matrix = db.movies.find_one({'title':'매트릭스'})
matrix_star = matrix['star']

result = list(db.movies.find({'star':matrix_star}))
for movie in result:
    print(movie['title'])

db.movies.update_one({'title':'매트릭스'},{'$set':{'star':'0'}})
