# - * - coding: utf-8 - * -

from bs4 import BeautifulSoup
import urllib2
import re
import time
import random

#修改header，设置成OS X系统下Chrome的agent
Headers = {'User-Agent':'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36'
}
#生成ip_list
url = 'http://www.xicidaili.com/nn/1'
req = urllib2.Request(url,headers=Headers)
res = urllib2.urlopen(req).read()
soup = BeautifulSoup(res)
#第一步处理
ips = soup.findAll('tr')
#获取ip列表
proxy_list = []
for x in range(1,len(ips)):
    ip = ips[x]
    tds = ip.findAll("td")
    tempString = tds[1].string + ':' + tds[2].string
    proxy_list.append(tempString.encode("UTF-8") )

'''proxy_list=[#这是我当时用的代理IP，请更新能用的IP
    '202.106.169.142:80',
    '220.181.35.109:8080',
    '124.65.163.10:8080',
    '117.79.131.109:8080',
    '58.30.233.200:8080',
    '115.182.92.87:8080',
    '210.75.240.62:3128',
    '211.71.20.246:3128',
    '115.182.83.38:8080',
    '121.69.8.234:8080',
        ]'''

# 最后输出的结点数量
num_node = 0
# 最后输出的边的数量
num_edge = 0

#这个函数求最小值
def MyMin(a,b):
    if a > b:
        return b
    else:
        return a

#这个函数得到top电影的url，把他们保存成一个list
def get_movie_URL(url_string,URL_List):
    for k in range(0,10):
        #绕过反爬虫的策略
        #time.sleep(0.5 + random.uniform(0,0.5))
        #随机选取一个ip绑定
        proxy = random.choice(proxy_list)
        urlhandle = urllib2.ProxyHandler({'http': proxy})
        opener = urllib2.build_opener(urlhandle)
        urllib2.install_opener(opener)

        request = urllib2.Request(url_string,headers = Headers)
        response = urllib2.urlopen(request)
        soup = BeautifulSoup(response.read())

        #得到符合要求的URL数据
        temp_URL_List = soup.find_all(name='a', href=re.compile("https://movie.douban.com/subject/") )
        #得到处理标签以得到链接
        temp_URL_List1  = [i.attrs['href'] for i in temp_URL_List if i.attrs['href'][0] != 'j']
        length = len(temp_URL_List)
        for i in range(0,length,2):
            URL_List.append(temp_URL_List1[i])

        #得到下一页的链接
        next_url = soup.find_all(name='link', href=re.compile("\?start=") )
        url_string = [i.attrs['href'] for i in next_url if i.attrs['href'][0] != 'j']
        url_string = "https://movie.douban.com/top250/" + url_string[len(url_string)-1]

    return URL_List

#该函数爬取每部电影的影评，并保存为一个一个list
#下标在3k位置的代表电影编号、下标在3k+1位置的代表评价的用户、下标在3k+2位置的代表该用户对电影的打分
def get_movie_reviews(url_string,info_list):

    num = url_string[len(url_string)-8:len(url_string)-1]
    url_string = url_string + "reviews"
    curr_url_string = url_string

    count = 0
    enough = False

    while curr_url_string:
        # 每隔0.3秒爬取一次，应对豆瓣反爬虫机制

        proxy = random.choice(proxy_list)
        urlhandle = urllib2.ProxyHandler({'http':proxy})
        opener = urllib2.build_opener(urlhandle)
        urllib2.install_opener(opener)

        request = urllib2.Request(curr_url_string,headers = Headers)
        response = urllib2.urlopen(request)
        soup = BeautifulSoup(response.read())


        #找到用户名列表
        name_list = soup.find_all(name='span', property = 'v:reviewer')
        #找到分数列表
        score_list = soup.find_all(name='span', property = 'v:rating')
        for j in range(0, MyMin(len(name_list),len(score_list)) ):
            info_list.append(num)
            info_list.append( name_list[j].string)
            tempString = score_list[j]['class'][0]
            info_list.append(tempString[7])
            count = count + 1
            if count >= 15:
                enough = True
                break

        if enough == True:
            break

            #找到下一页的链接
        next_url = soup.find_all(name='link', href=re.compile("\?start="), rel='next')
        if len(next_url) == 0:
            break
        else:
            curr_url_string = [i.attrs['href'] for i in next_url if i.attrs['href'][0] != 'j']
            curr_url_string = url_string + curr_url_string[0]

    return info_list

#函数参数是每个电影影评的列表，返回一个题目要求输出的列表
#下标在3k位置是第一个影评者的ID、下标在3k+1位置是第二个影评者的ID、下标在3k+2位置的二者的权值
#这个形式类似于无向图的边列表
def node_list(info_list,final_list):

    #先给每个影评人去分配唯一的一个编号(从0开始计数）
    nameID_list = []
    nameID_list.append(0)
    startID = 1
    for i in range(4,len(info_list),3):
        #初始设置为没有重复
        isDu = False
        for j in range(1,i,3):
            #如果重复了
            if info_list[i] == info_list[j]:
                # 拿之前的编号给这个编号标号
                isDu = True
                nameID_list.append(nameID_list[j/3])
                break
        #如果没有重复
        if isDu == False:
            nameID_list.append(startID)
            startID = startID + 1
    #更新结点的数量
    num_node = startID


    for i in range(0,len(info_list),3):

        for j in range(i+3,i+1000,3):

            #如果数组越界，结束循环
            if j >= len(info_list):
                break

            if info_list[i] == info_list[j]: #如果两个人看的影片相同
                # 在final_list里寻找二者是否已经建立好了结点
                ID_1 = nameID_list[i/3]
                ID_2 = nameID_list[j/3]
                hasSet = False
                for k in range(0,len(final_list),3):
                    # 如果在final_list里二者已经建立好的结点
                    if final_list[k] == ID_1 and final_list[k+1] == ID_2:
                        hasSet = True
                        prev_big = int(final_list[k+2])
                        prev_small = final_list[k+2] - prev_big
                        curr_small = (prev_big * prev_small + 1 - (abs(ord(info_list[i+2])-ord(info_list[j+2])) + 1) * 0.8 / 5) / (prev_big+1)
                        curr_big = prev_big + 1
                        final_list[k+2] = curr_big + curr_small
                        break
                    elif final_list[k] == ID_2 and final_list[k+1] == ID_1:
                        hasSet = True
                        prev_big = int(final_list[k + 2])
                        prev_small = final_list[k + 2] - prev_big
                        curr_small = (prev_big * prev_small + 1 - (abs(ord(info_list[i+2])-ord(info_list[j+2])) + 1) * 0.8 / 5) / (
                        prev_big + 1)
                        curr_big = prev_big + 1
                        final_list[k + 2] = curr_big + curr_small
                        break

                # 如果在final_list里二者没有建立好的结点
                if hasSet == False:
                    #新建结点
                    final_list.append(ID_1)
                    final_list.append(ID_2)
                    #真是复杂的计算相似度的公式。。。
                    final_list.append(1 + 1 - (abs(ord(info_list[i+2])-ord(info_list[j+2])) + 1) * 0.8 / 5 )



            else:#如果两个人看的影片不同，跳出循环去遍历下一个人的影评
                break

    #更新边的数量
    num_edge = len(final_list)/3
    #把边与点的数据记录在列表的最后
    final_list.append(num_node)
    final_list.append(num_edge)
    final_list.append(0)

    return final_list

# 该函数把final_list的数据，打印成题目要求的格式存在一个txt里
def print_txt(final_list):
    #打开文件
    f = open("movie.txt", 'w')

    length = len(final_list)
    #输入要求的第一行
    f.write(str(final_list[length-3]))
    f.write(" ")
    f.write(str(final_list[length-2]))
    f.write("\n")
    #输出剩下的行
    for i in range(0,len(final_list)-3,3):
        f.write(str(final_list[i]))
        f.write(" ")
        f.write(str(final_list[i+1]))
        f.write(" ")
        f.write(str(final_list[i+2]))
        f.write("\n")

    #关闭文件
    f.close()

    return

#函数的main方法
def main():

    # 得到豆瓣电影top250的每个的url
    url_string = "https://movie.douban.com/top250"
    url_list = []c
    url_list = get_movie_URL(url_string,url_list)

    info_list = []
    for i in range(0,len(url_list)):
        url_string = url_list[i]
        info_list = get_movie_reviews(url_string, info_list)
        if i >= 20:
            breakc

    #最后一次性处理整个数据

    final_list = []
    final_list = node_list(info_list, final_list)

    #打印结果
    print_txt(final_list)

    return


main()

