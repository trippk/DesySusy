



def BuildDictStructure(Samples):
    """builds the dictionary structure that holds a set of histograms
    with their properties. It is now empty, to be filled with the histos
    and property dictionaries later on"""
    KeyList=[]
    if type(Samples)==dict:
        for key,values in Samples.iteritems():
            KeyList.append(key)
    else:
        KeyList.append(Samples.SampleName)

    #
    StructureDictionary={}
    for key in KeyList:
        #
        #the item is a list
        StructureDictionary[key]=[]
        #the first entry of the list will have to be the histogram pointer
        #I put 'nil' but I will overwrite it with the pointer later on.
        StructureDictionary[key].append('nil')
        #
        #the second entry of the list is a dictionary of plot properties
        #also set to emtpy and to be filled later on
        PlotPropertiesDictionary={}
        StructureDictionary[key].append(PlotPropertiesDictionary)
    #
    return StructureDictionary
