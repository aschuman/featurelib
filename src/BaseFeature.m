%BaseFeature MATLAB class wrapper to underlying C++ class
classdef BaseFeature < handle
    properties (SetAccess = private, Hidden = true)
        objectHandle; % Handle to the underlying C++ class instance
    end
	
    methods
        % Constructor - Create a new C++ class instance 
        function this = BaseFeature(varargin)
            this.objectHandle = BaseFeatureMex('new', varargin{:});
        end
        
        % Destructor - Destroy the C++ class instance
        function delete(this)
            BaseFeatureMex('delete', this.objectHandle);
        end

        % Compute
        function varargout = compute(this, varargin)
            [varargout{1:nargout}] = BaseFeatureMex('compute', this.objectHandle, varargin{:});
        end

        % Distance
        function varargout = distance(this, varargin)
            [varargout{1:nargout}] = BaseFeatureMex('distance', this.objectHandle, varargin{:});
        end
        
        % Set parameters
        function varargout = setParameters(this, varargin)
            [varargout{1:nargout}] = BaseFeatureMex('setParameters', this.objectHandle, varargin{:});
        end
        
        % Write to file
        function varargout = writeToFile(this, varargin)
            path = varargin{3};
            path = strrep(path,'\',filesep);
            path = strrep(path,'/',filesep);
            varargin{3} = path;
            [varargout{1:nargout}] = BaseFeatureMex('writeToFile', this.objectHandle, varargin{:});
        end
        
        % Read from file
        function varargout = readFromFile(this, varargin)
            path = varargin{3};
            path = strrep(path,'\',filesep);
            path = strrep(path,'/',filesep);
            varargin{3} = path;
            [varargout{1:nargout}] = BaseFeatureMex('readFromFile', this.objectHandle, varargin{:});
        end
    end
end