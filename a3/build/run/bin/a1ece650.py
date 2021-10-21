#!/usr/bin/env python3
import sys
import re
from math import sqrt


# YOUR CODE GOES HERE
class Point:
    def __init__(self, x, y):
        self.x = round(float(x),2)
        self.y = round(float(y),2)


class Line:
    def __init__(self, start, end):
        self.start = start
        self.end = end


class Street_DB:
    # intersection_dict = {}

    def __init__(self):
        self.street_dict = {}
        self.edge_list = []
        self.vertices_list = []
        self.intersection_dict = {}

    def add(self, street_name, line_list):
        if street_name in self.street_dict:
            sys.stderr.write("Error: This street has already been added.")
            #print("Error: This street has already been added.")
        else:
            self.street_dict[street_name] = line_list  # adds the lines (street segments) and street to street_dict

    def remove(self, street_name):
        #print(street_name)
        if street_name not in self.street_dict:
            sys.stderr.write("Error: This street has not been added.")
            #print("Error: This street has not been added.")
        else:
            del self.street_dict[street_name]  # removes the street from the dict

    def change(self, street_name, line_list):
        if street_name in self.street_dict:
            self.street_dict[street_name] = line_list  # changes the street line (street segments) for a given street
            #print("new street line is : {} {}".format(line_list[0].start, line_list[0].end))
        else:
            sys.stderr.write("Error: This street does not exist")
            #print("Error: This street does not exist")

    def graph(self):
        self.intersection_dict = {}

        values = list(self.street_dict.values())
        data = []
        for i, val in enumerate(values):
            for v in val:
                for j, x in enumerate(values):
                    for l in x:
                        if (j != i) and ((l, v) not in data):
                            data.append((v, l))
                            #print("lines are: {}{} \t {}{}".format(v.start, v.end, l.start, l.end))

        #print(len(data))

        for i in data:
            temp_list = []  # to handle multiple lines with same intersection point
            re_value = intersection(i)
           # print("intersection between {} {}  and  {} {} is {}".format(i[0].start,i[0].end,i[1].start,i[1].end,re_value))
            if re_value is not None and re_value !=[]:
                if type(re_value) is list:
                   for v in re_value:
                        if v not in self.intersection_dict:  # if no intersection exists add a new one
                            self.intersection_dict[v] = i
                        elif v in self.intersection_dict:
                            if type(self.intersection_dict[v]) is list:
                                temp_list.extend(
                                    self.intersection_dict[v])  # if intersection exists then add old line to temp
                                temp_list.append(i)
                            else:
                                temp_list.append(self.intersection_dict[v])  # add new lines for same intersection by extending the list
                                temp_list.append(i)
                            self.intersection_dict[v] = temp_list

                else:
                    if re_value not in self.intersection_dict:  # if no intersection exists add a new one
                        self.intersection_dict[re_value] = i
                    elif re_value in self.intersection_dict:
                        if type(self.intersection_dict[re_value]) is list:
                            temp_list.extend(
                                self.intersection_dict[re_value])  # if intersection exists then add old line to temp
                            temp_list.append(i)
                        else:
                            temp_list.append(self.intersection_dict[
                                                 re_value])  # add new lines for same intersection by extending the list
                            temp_list.append(i)
                        self.intersection_dict[re_value] = temp_list  # extended list becomes the new value of the dict

        #print(self.intersection_dict)

        self.vertices_list = get_vertices(self.intersection_dict)
        self.edge_list = get_edges(self.vertices_list, self.intersection_dict)
        '''
        for i in self.intersection_dict:
            if type(self.intersection_dict[i]) is list:
                for k in range(len(self.intersection_dict[i])):
                    print("{} intersects between {} {} and {} {}".format(i, self.intersection_dict[i][k][0].start,
                                                                         self.intersection_dict[i][k][0].end,
                                                                         self.intersection_dict[i][k][1].start,
                                                                         self.intersection_dict[i][k][1].end))
            else:
                print("{} intersects between {} {} and {} {}".format(i, self.intersection_dict[i][0].start,
                                                                     self.intersection_dict[i][0].end,
                                                                     self.intersection_dict[i][1].start,
                                                                     self.intersection_dict[i][1].end))
        '''
        #print(self.vertices_list)
        #print(self.edge_list)
        #print(self.intersection_dict)
        map_print(self.vertices_list, self.edge_list)


def map_print(vertices, edges):
    
    print("V  {}".format(len(vertices)))
    sys.stdout.flush()

    edge_print = []

    for e in range(len(edges)):
        for i in range(len(vertices)):
            if edges[e][0] == vertices[i]:
                start_index = i 
                for d, x in enumerate(vertices):
                    if edges[e][1] == x:
                        end_index = d
                        edge_print.append((start_index, end_index))

    print('E {', end = '')
    
    for i, e in enumerate(edge_print):
        if i != (len(edge_print) - 1):
            print("<{},{}>,".format(e[0], e[1]),end  = "")
        else:
            print("<{0},{1}>".format(e[0], e[1]),end = "")
            print("}")
    sys.stdout.flush()
    #print("}")


#    pass

def distance(point_list, line_start, line_end):
    d_p_start = []
    d_p_end = []
    d_start_end = []
    result = []

    for p in point_list:
        d_p_start.append(sqrt((line_start[0] - p[0]) ** 2 + (line_start[1] - p[1]) ** 2))
        d_p_end.append(sqrt((line_end[0] - p[0]) ** 2 + (line_end[1] - p[1]) ** 2))
        d_start_end.append(sqrt((line_end[0] - line_start[0]) ** 2 + (line_end[1] - line_start[1]) ** 2))

    for i in range(len(point_list)):
        if (round((d_p_start[i] + d_p_end[i]),4) == round(d_start_end[i],4)):
            result.append(1)
        else:
            result.append(0)

    for i in result:
        if i == 1:
            return 1

    return 0

# def distance(point_list, line_start, line_end):
#    # print("point list",point_list)
#     #print(line_start,line_end)
#     result = []
#     if(line_end[0] == line_start[0]):
#         for p in point_list:
#             if (line_end[1] > line_start[1]):
#                 if (line_start[1] <= p[1] <= line_end[1]):
#                     result.append(1)
#                 else:
#                     result.append(0)
#             else:
#                 if (line_end[1] <= p[1] <= line_start[1]):
#                     result.append(1)
#                 else:
#                     result.append(0)
#     else:
#         m = (line_end[1]-line_start[1])/(line_end[0]-line_start[0])
#         c = line_end[1] - m * line_end[0]
#         for p in point_list:
#            # print(p[1],p[0])
#             # if p satisfies y = mx+c then it lies on the line
#             if (p[1] == round((m * p[0] + c),2)):
#                 result.append(1)
#             else:
#                 result.append(0)
#     #print(result)
#     for i in result:
#         if i == 1:
#             return 1
#     return 0


def get_edges(vertices, inter_dict):
    edges = []
    intersections = list(inter_dict)
    temp_lines = list(inter_dict.values())
    lines = []

    for i in temp_lines:
        for k in i:
            if type(i) is list:
                lines.extend(k)
            else:
                lines.append(k)

    for i in intersections:
        #print("i: {}".format(i))
        for l in lines:
            if type(inter_dict[i]) is list:
                for k in range(len(inter_dict[i])):
                    if l in inter_dict[i][k] or (l == inter_dict[i][k]):
         #               print("l : {} {}".format(l.start, l.end))
                        for v in vertices:
          #                  print("v : {} ".format(v))
                            if v != i:
                                if distance([x for x in vertices if x != l.start and x != l.end and x != i], l.start,
                                            l.end) != 1 and (i != l.start) and (i != l.end):  # if other vertices are not on the line being checked
                                    edges.append((i, l.start))
                                    edges.append((i, l.end))
           #                         print("1. if v is not on l. \t i: {} and l.start: {}".format(i, l.start))
            #                        print("2. if v is not on l. \t i: {} and l.end: {}".format(i, l.end))

                                else:  # if other vertex is on the line and vertex is not the same intersection
                                    if distance([x for x in vertices if x != i and x != l.start], i, l.start) != 1 and (
                                            i != l.start):  # vertex is not between intersection and start of line
                                        edges.append((i, l.start))
             #                           print("3. if v is not between i and l.start\t i: {} and l.start: {}".format(i,l.start))
                                    elif distance([x for x in vertices if x != i and x != l.end], i, l.end) != 1 and (
                                            i != l.end):
                                        edges.append((i, l.end))
              #                          print("4. if v is not between i and l.end\t i: {} and l.end: {}".format(i, l.end))
                                    elif (distance([x for x in vertices if x != i and x != v], i, v) != 1) and (distance([v],l.start,l.end) == 1):
                                         edges.append((i, v))
               #                         print("5. if no other vertices are between i and v\t i: {} and v: {}".format(i, v))

            elif l in inter_dict[i] or (l == inter_dict[i]):
               # print("ELSEIF-- l : {} {}".format(l.start, l.end))
                for v in vertices:
                #    print("ELSEIF-- v : {}".format(v))
                    if v != i:
                        d = distance([x for x in vertices if (x != l.start) and (x != l.end) and (x != i)], l.start,
                                    l.end)
                 #       print("Distance is {}".format(d))
                        if d != 1 and (i != l.start) and (i != l.end):  # if other vertices are not on the line being checked
                            edges.append((i, l.start))
                            edges.append((i, l.end))
                  #          print("ELSEIF -- if v is not on l. \t i: {} and l.start: {}".format(i, l.start))
                   #         print("ELSEIF -- if v is not on l. \t i: {} and l.end: {}".format(i, l.end))
                        else:  # if other vertex is on the line and vertex is not the same intersection
                           # d1 = distance([x for x in vertices if x != i and x != l.start], i, l.start)
                            #d2 = distance([x for x in vertices if x != i and x != l.end], i, l.end)
                            #d3 = distance([x for x in vertices if x != i and x != v], i, v)
                    #        print("Distance leaving i, l.start  is {}".format(d1))
                     #       print("Distance leaving i, l.end  is {}".format(d2))
                      #      print("Distance leaving i, v  is {}".format(d3))
                            if distance([x for x in vertices if x != i and x != l.start], i, l.start) != 1 and (i != l.start):  # vertex is not between intersection and start of line
                                edges.append((i, l.start))
                       #         print("ELSEIF -- if v is not between i and l.start\t i: {} and l.start: {}".format(i, l.start))
                            elif distance([x for x in vertices if x != i and x != l.end], i, l.end) != 1 and (
                                    i != l.end):
                                edges.append((i, l.end))
                        #        print("ELSEIF -- if v is not between i and l.end\t i: {} and l.end: {}".format(i, l.end))
                            elif (distance([x for x in vertices if x != i and x != v], i, v) != 1) and (distance([v],l.start,l.end) == 1):
                                edges.append((i, v))
                         #       print("ELSEIF -- if no other vertices are between i and v\t i: {} and v: {}".format(i, v))

        for v in inter_dict:
            if distance([x for x in vertices if x != i and x != v], i, v) != 1 and (v != i) and (
                    (v, i) not in edges):  # appending edge between intersections
                for m in inter_dict[i]: # for all the lines that make up the intersection i
                    if type(m) is list:     # if the more than 2 lines make up the intersection then it is a list
                        for j in m:         # for a line in the list of lines that make up the intersection
                            for a in inter_dict[v]:  # for all the lines that make up the other intersection
                                if type(a) is list: # if v is also made up by a list of lines (>2)
                                    for b in a:     # for each line in that list of lines
                                        if j == b:  # if any line making v is in the list of lines making i
                                            if distance([x for x in inter_dict if x != v and x != i ],j.start,j.end) != 1:
                                       #         print("adding 1", (i, v))
                                                edges.append((i, v))   # append i,v since they lie on the same line and no vertex between them
                                else:   #if v is made by just 2 lines
                                    if j == a: # if one of the lines in list intersecting i is one of the two lines intersecting v
                                       # print("adding 2", (i, v))
                                        edges.append((i, v))
                    else:   #if the intersection i is made by only two lines
                        for c in inter_dict[v]:  # for all the lines that make up the other intersection
                            if type(c) is list: # if more than 2 lines intersect to make v
                                for d in c:    # for a line in the list of lines making v
                                    if m == d:  # if the one of the lines making i is in the list making v
                                 #       print("adding 3", (i, v))
                                        edges.append((i, v))
                            else:
                                if m == c:
                                    edges.append((i, v))

    real_edges = []

    for i in edges:
        if i not in real_edges:
            real_edges.append(i)
        if (i[1], i[0]) in real_edges: # if edge exists in reverse form delete it
            real_edges.remove(i)
    return real_edges


def get_vertices(inter_dict):
    vertices = []

    intersections = list(inter_dict)
    vertices.extend(intersections)  # need to add intersections to vertices list

    lines = list(inter_dict.values())
    # print(lines)
    for i in lines:
        if type(i) is list:
            for k in i:
                vertices.append(k[0].start)
                vertices.append(k[0].end)
                vertices.append(k[1].start)
                vertices.append(k[1].end)
        else:
            vertices.append(i[0].start)
            vertices.append(i[0].end)
            vertices.append(i[1].start)
            vertices.append(i[1].end)

    temp_list = []
    for i in vertices:
        if i not in temp_list:
            temp_list.append(i)
    vertices = temp_list

    return vertices


def intersection(lines):

    x1, y1 = lines[0].start[0], lines[0].start[1]
    x2, y2 = lines[0].end[0], lines[0].end[1]
    x3, y3 = lines[1].start[0], lines[1].start[1]
    x4, y4 = lines[1].end[0], lines[1].end[1]

    list1_x = [x1, x2]
    list2_x = [x3, x4]

    list1_x.sort()
    list2_x.sort()

    list1_y = [y1, y2]
    list2_y = [y3, y4]

    list1_y.sort()
    list2_y.sort()

    # min and max of the 2 segment's x and y coordinates
    xmin_seg1 = min(list1_x)
    xmax_seg1 = max(list1_x)
    xmin_seg2 = min(list2_x)
    xmax_seg2 = max(list2_x)
    ymin_seg1 = min(list1_y)
    ymax_seg1 = max(list1_y)
    ymax_seg2 = max(list2_y)
    ymin_seg2 = min(list2_y)

    interval_x = (max(xmin_seg1, xmin_seg2), min(xmax_seg1, xmax_seg2))
    interval_y = (max(ymin_seg1, ymin_seg2), min(ymax_seg1, ymax_seg2))


    if x1 == x2 == x3 == x4: #check for overlapping lines
        points = [(x1, y1), (x2, y2), (x3, y3), (x4, y4)]
        temp = []
        for p in points:
            if interval_y[0] <= p[1] <= interval_y[1]:
                temp.append(p)
        return temp

    elif x1 != x2 and x3 != x4:
        m1 = (y2 - y1) / (x2 - x1)
        m2 = (y4 - y3) / (x4 - x3)

        b1 = y1 - m1 * x1
        b2 = y3 - m2 * x3

        if m1 == m2 and b1 == b2:
            points = [(x1, y1), (x2, y2), (x3, y3), (x4, y4)]
            temp = []
            for p in points:
                if (interval_x[0] <= p[0] <= interval_x[1] and
                        interval_y[0] <= p[1] <= interval_y[1]):
                    temp.append(p)
            return temp

    x_num = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    y_num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    x_den = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    y_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)

    try:
        x_coor = x_num / x_den
        y_coor = y_num / y_den
    except ZeroDivisionError:
        return []

    if (x_coor < interval_x[0] or x_coor > interval_x[1] or
            y_coor < interval_y[0] or y_coor > interval_y[1]):
        return []

    return (round(x_coor, 2), round(y_coor, 2))



def parseInput(line):
    # st_pattern = "(.*?)"

    line_split = re.split('[" ]', line) #line_split = line.strip().split()

    if len(line_split) > 1:  # if more than one argument then separate into cmd, street_name  and coordinates (points)
        cmd = line_split[0]  # commands 'a','c','r' as the first element of the input

        street = re.findall(r' "([a-zA-Z\s]*)" ', line)# regex returns street name between " " as a list
        #print(street)

        if street != []:
            street = street[0].upper()
        elif street == [] and cmd == 'r':
            street = re.findall(r' "([a-zA-Z\s]*)"', line)
            street = street[0].upper()
        else:
            pass

        coordinates = re.findall(r'\((-?\d+),(-?\d+)\)', line)  # regex returns coordinates but as strings
        if coordinates != []:
            coordinates = coordinates
        else:
            pass

        bkt_open = sum(map(lambda x: 1 if '(' in x else 0, line))
        bkt_close = sum(map(lambda x: 1 if ')' in x else 0, line))
        #print("street being returned is {}".format(street))
        #print(cmd, street, coordinates)
        return cmd, street, coordinates, bkt_open, bkt_close #return cmd, street, coordinates



def main():
    # YOUR MAIN CODE GOES HERE

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment

    street_db = Street_DB()
    while True:
        input_line = sys.stdin.readline()

       # plist = []
        line_split_main = input_line.strip().split()  # line split in main func

        if input_line == '':
            break
        elif len(line_split_main) > 1:
            cmd, street, coordinates, bkt_open, bkt_close = parseInput(input_line) #cmd, street, coordinates = parseInput(input_line)
            # coordinate_dict[street] = coordinates       # adding street as key and coordinates as value to a dict

            if cmd == 'a':
                new_line_list = []
                #print(street)
                for i in range(len(coordinates) - 1):
                    p_start = Point(coordinates[i][0],
                                    coordinates[i][1])  # starting point of the street from coordinates
                    p_end = Point(coordinates[i + 1][0],
                                  coordinates[i + 1][1])  # ending point of the street from coordinates
                    line = Line((p_start.x, p_start.y),
                                (p_end.x, p_end.y))  # makes a line segment with 2 points from the same street
                    new_line_list.append(line)  # adding line segment to street_db's street_list

                if street == []:
                      # adding lines and street to street_db dict using add
                    sys.stderr.write("Error: Please check your input street. It needs to be within double-quotes and spaced out after/before command/coordinates respectively.")
                   # print("Error: Please check your input street. It needs to be within double-quotes and spaced out after/before command/coordinates respectively.")
                elif (coordinates == []) or (bkt_open != bkt_close):  #elif coordinates == []:
                    sys.stderr.write("Error: Please check your coordinates.")
                    #print("Error: Please check your coordinates.")
                else:
                    street_db.add(street, new_line_list)

            elif cmd == 'c':
                # same as if cmd == 'a' but for changing/overwriting existing street
                new_l = []
               # print(street)
                for i in range(len(coordinates) - 1):
                    p_start = Point(coordinates[i][0],
                                    coordinates[i][1])  # starting point of the street from coordinates
                    p_end = Point(coordinates[i + 1][0],
                                  coordinates[i + 1][1])  # ending point of the street from coordinates
                    line = Line((p_start.x, p_start.y),
                                (p_end.x, p_end.y))  # makes a line segment with 2 points from the same street
                    new_l.append(line)  # adding line segment to street_db's street_list

                #if street.lower() in

                if street == []:
                      # adding lines and street to street_db dict using add
                    sys.stderr.write("Error: Please check your input street. It needs to be within double-quotes and spaced out after/before command/coordinates respectively.")
                    #print("Error: Please check your input street. It needs to be within double-quotes and spaced out after/before command/coordinates respectively.")
                elif coordinates == [] or (bkt_open != bkt_close):
                    sys.stderr.write("Error: Please check your coordinates.")
                    #print("Error: Please check your coordinates.")
                else:
                    street_db.change(street, new_l) # changing street in dict to different points

            elif cmd == 'r':
               # print("street is {}".format(street))
                street_db.remove(street)  # removing street from dict

            else:
                sys.stderr.write("Error: Bad input!")
                #print("Error: Bad input!")

        elif len(line_split_main) == 1 and line_split_main[0] == 'g':  # checking if input is 1 char and == g
            street_db.graph()  # Calling graph function
        else:
            sys.stderr.write("Error: too few arguments or wrong command entered.")
            #print("Error: too few arguments or wrong command entered.")

        # print(street_db.street_dict)

    #print("Finished reading input")
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()

# for c in coordinates:
#    # print(c[0],c[1])
#     p = Point(c[0],c[1])
#     point_list.append(p)
